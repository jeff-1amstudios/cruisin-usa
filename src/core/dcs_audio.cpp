#include "dcs_audio.h"

#include "DCSDecoderNative.h"
#include "miniz.h"

#include <algorithm>
#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <new>
#include <string>
#include <vector>

namespace {

constexpr unsigned kFirstSoundChip = 2;
constexpr unsigned kLastSoundChip = 9;
constexpr size_t kSoundRomSize = 512 * 1024;
constexpr size_t kEngineStart = 0x4000;
constexpr size_t kEngineEnd = 0x1D368;

struct OwnedTrack {
    std::vector<int16_t> pcm;
    dcs_pcm_track view{};
};

void SetError(char* error, size_t errorSize, const char* format, ...)
{
    if (error == nullptr || errorSize == 0)
        return;

    va_list args;
    va_start(args, format);
    std::vsnprintf(error, errorSize, format, args);
    va_end(args);
}

bool LoadSoundROMs(const char* zipPath,
    std::array<std::vector<uint8_t>, kLastSoundChip + 1>& roms,
    char* error, size_t errorSize)
{
    mz_zip_archive archive{};
    if (!mz_zip_reader_init_file(&archive, zipPath, 0)) {
        SetError(error, errorSize, "unable to open %s", zipPath);
        return false;
    }

    bool ok = true;
    for (unsigned chip = kFirstSoundChip; chip <= kLastSoundChip; ++chip) {
        char suffix[16];
        std::snprintf(suffix, sizeof(suffix), "_u%u.u%u", chip, chip);
        int found = -1;
        const mz_uint files = mz_zip_reader_get_num_files(&archive);
        for (mz_uint index = 0; index < files; ++index) {
            mz_zip_archive_file_stat stat{};
            if (!mz_zip_reader_file_stat(&archive, index, &stat))
                continue;
            const std::string name(stat.m_filename);
            if (name.size() >= std::strlen(suffix)
                && name.compare(name.size() - std::strlen(suffix), std::strlen(suffix), suffix) == 0
                && name.find("sound_rom") != std::string::npos) {
                found = static_cast<int>(index);
                break;
            }
        }

        if (found < 0) {
            SetError(error, errorSize, "sound ROM U%u is missing from %s", chip, zipPath);
            ok = false;
            break;
        }

        roms[chip].resize(kSoundRomSize);
        if (!mz_zip_reader_extract_to_mem(&archive, static_cast<mz_uint>(found),
                roms[chip].data(), roms[chip].size(), 0)) {
            SetError(error, errorSize, "unable to extract sound ROM U%u", chip);
            ok = false;
            break;
        }
    }

    mz_zip_reader_end(&archive);
    return ok;
}

} // namespace

struct dcs_pcm_bank {
    std::vector<OwnedTrack> tracks;
    OwnedTrack engine;
    size_t decodedTracks = 0;
    size_t decodedSamples = 0;
};

extern "C" dcs_pcm_bank* dcs_audio_decode_roms(const char* zipPath, char* error, size_t errorSize)
{
    if (zipPath == nullptr) {
        SetError(error, errorSize, "no DCS ROM zip path was supplied");
        return nullptr;
    }

    std::array<std::vector<uint8_t>, kLastSoundChip + 1> roms;
    if (!LoadSoundROMs(zipPath, roms, error, errorSize))
        return nullptr;

    DCSDecoder::MinHost host;
    DCSDecoderNative decoder(&host);
    for (unsigned chip = kFirstSoundChip; chip <= kLastSoundChip; ++chip)
        decoder.AddROM(static_cast<int>(chip), roms[chip].data(), roms[chip].size());

    const uint8_t check = decoder.CheckROMs();
    if (check != 1) {
        SetError(error, errorSize, "DCS ROM validation failed at U%u", check);
        return nullptr;
    }

    dcs_pcm_bank* bank = new (std::nothrow) dcs_pcm_bank;
    if (bank == nullptr) {
        SetError(error, errorSize, "out of memory allocating the DCS PCM bank");
        return nullptr;
    }

    try {
        bank->tracks.resize(static_cast<size_t>(decoder.GetMaxTrackNumber()) + 1);
        decoder.SoftBoot();
        decoder.SetMasterVolume(255);

        for (unsigned number = 0; number <= decoder.GetMaxTrackNumber(); ++number) {
            DCSDecoder::TrackInfo info;
            if (!decoder.GetTrackInfo(static_cast<uint16_t>(number), info)
                || info.type != 1)
                continue;

            const auto program = decoder.DecompileTrackProgram(static_cast<uint16_t>(number));
            bool hasStream = false;
            bool streamLoopsForever = false;
            size_t streamFrameCount = 0;
            for (const auto& opcode : program) {
                if (opcode.opcode == 0x01) {
                    hasStream = true;
                    const uint32_t address =
                        (static_cast<uint32_t>(opcode.operandBytes[1]) << 16)
                        | (static_cast<uint32_t>(opcode.operandBytes[2]) << 8)
                        | static_cast<uint32_t>(opcode.operandBytes[3]);
                    const size_t frames = decoder.MakeROMPointer(address).PeekU16();
                    unsigned repeats = opcode.operandBytes[4];
                    if (repeats == 0) {
                        streamLoopsForever = true;
                        repeats = 1;
                    }
                    streamFrameCount = std::max(streamFrameCount, frames * repeats);
                }
            }
            if (!hasStream)
                continue;

            OwnedTrack& track = bank->tracks[number];
            // A track can start a finite stream and then Wait(Forever).  DCSExplorer
            // reports those programs as time=0 and looping=true, even though their
            // audio is the full finite stream rather than an empty program loop.
            // Use the referenced stream length for that case.  STARTLINEREVS2
            // (track 626) is one such program: its stream is 719 frames long.
            const size_t frameCount = info.time != 0 ? info.time : streamFrameCount;
            const size_t sampleCount = (frameCount + 2) * 240;
            track.pcm.resize(sampleCount);
            decoder.ClearTracks();
            decoder.AddTrackCommand(static_cast<uint16_t>(number));
            for (size_t sample = 0; sample < sampleCount; ++sample)
                track.pcm[sample] = decoder.GetNextSample();

            const bool looping = streamLoopsForever || (info.looping && info.time != 0);
            track.view = { track.pcm.data(), track.pcm.size(), looping ? 1 : 0 };
            ++bank->decodedTracks;
            bank->decodedSamples += track.pcm.size();
        }

        // Cruis'n's 55CC handler reads a dedicated little-endian PCM region
        // from U2, advancing a persistent cursor at the requested engine rate.
        const auto& u2 = roms[2];
        bank->engine.pcm.reserve((kEngineEnd - kEngineStart) / 2);
        for (size_t offset = kEngineStart; offset + 1 < kEngineEnd; offset += 2) {
            const uint16_t sample = static_cast<uint16_t>(u2[offset])
                | (static_cast<uint16_t>(u2[offset + 1]) << 8);
            bank->engine.pcm.push_back(static_cast<int16_t>(sample));
        }
        bank->engine.view = {
            bank->engine.pcm.data(), bank->engine.pcm.size(), 1
        };
    } catch (const std::bad_alloc&) {
        delete bank;
        SetError(error, errorSize, "out of memory while decoding DCS audio");
        return nullptr;
    }

    if (error != nullptr && errorSize != 0)
        error[0] = '\0';
    return bank;
}

extern "C" void dcs_audio_free(dcs_pcm_bank* bank)
{
    delete bank;
}

extern "C" const dcs_pcm_track* dcs_audio_track(const dcs_pcm_bank* bank, unsigned number)
{
    if (bank == nullptr || number >= bank->tracks.size() || bank->tracks[number].pcm.empty())
        return nullptr;
    return &bank->tracks[number].view;
}

extern "C" const dcs_pcm_track* dcs_audio_engine_loop(const dcs_pcm_bank* bank)
{
    return bank == nullptr || bank->engine.pcm.empty() ? nullptr : &bank->engine.view;
}

extern "C" size_t dcs_audio_decoded_track_count(const dcs_pcm_bank* bank)
{
    return bank == nullptr ? 0 : bank->decodedTracks;
}

extern "C" size_t dcs_audio_decoded_sample_count(const dcs_pcm_bank* bank)
{
    return bank == nullptr ? 0 : bank->decodedSamples;
}
