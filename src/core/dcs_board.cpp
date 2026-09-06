#include "dcs_board.h"

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define MSB_FIRST
#else
#define LSB_FIRST
#endif
#define UINT32 uint32_t
#define HAS_ADSP2101 1
#define HAS_ADSP2105 1
#include "adsp2100/adsp2100.h"
#include "miniz.h"

#include <algorithm>
#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <deque>
#include <new>
#include <string>
#include <vector>

#undef HAS_ADSP2105
#undef HAS_ADSP2101
#undef UINT32
#undef LSB_FIRST
#undef MSB_FIRST

namespace {

constexpr unsigned kFirstSoundChip = 2;
constexpr unsigned kLastSoundChip = 9;
constexpr size_t kSoundRomSize = 512 * 1024;
constexpr size_t kSocketSpacingWords = 0x100000;
constexpr size_t kSoundRegionWords = 0x800000;
constexpr int kAdspClock = 10000000;
constexpr int kCyclesPerSample = kAdspClock / DCS_BOARD_SAMPLE_RATE;

struct BoardState {
    std::array<uint32_t, 0x4000> program{};
    std::array<uint16_t, 0x4000> data{};
    std::vector<uint16_t> sound;
    std::deque<int16_t> output;
    uint32_t bank = 0;
    uint16_t input = 0;
    bool inputFull = false;
    bool autobuffer = false;
    int ireg = 0;
    int increment = 0;
    int size = 0;
    int base = 0;
    int halfSamples = 0;
    int cyclesUntilDma = 0;
};

BoardState* gBoard = nullptr;

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
                && name.compare(name.size() - std::strlen(suffix),
                    std::strlen(suffix), suffix) == 0
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

uint16_t ReadU16BE(const uint8_t* source)
{
    return static_cast<uint16_t>((source[0] << 8) | source[1]);
}

uint16_t DcsChecksum(const std::vector<uint8_t>& rom)
{
    uint16_t even = 0;
    uint16_t odd = 0;
    for (size_t offset = 0; offset + 1 < rom.size(); offset += 2) {
        even = static_cast<uint16_t>(even + rom[offset]);
        odd = static_cast<uint16_t>(odd + rom[offset + 1]);
    }
    return static_cast<uint16_t>(((even << 8) & 0xff00) | (odd & 0x00ff));
}

bool ValidateSoundROMs(
    const std::array<std::vector<uint8_t>, kLastSoundChip + 1>& roms,
    char* error, size_t errorSize)
{
    static constexpr char kSignature[] = "Cruis'n USA (c)1994 Midway";
    const auto& u2 = roms[kFirstSoundChip];
    if (u2.size() < 0x24000 + 9 * 6
        || std::memcmp(u2.data() + 4, kSignature, sizeof(kSignature) - 1) != 0) {
        SetError(error, errorSize, "sound ROM U2 is not a Cruis'n USA DCS image");
        return false;
    }

    const uint8_t* entry = u2.data() + 0x24000;
    for (unsigned index = 0; index <= kLastSoundChip - kFirstSoundChip; ++index) {
        const unsigned chip = index + kFirstSoundChip;
        const uint32_t size = static_cast<uint32_t>(ReadU16BE(entry)) * 4096;
        const unsigned bank = ReadU16BE(entry + 2) >> 8;
        const uint16_t expectedChecksum = ReadU16BE(entry + 4);
        const uint16_t actualChecksum = index == 0 ? 0 : DcsChecksum(roms[chip]);
        if (size != roms[chip].size() || bank != index
            || expectedChecksum != actualChecksum) {
            SetError(error, errorSize, "sound ROM U%u failed DCS catalog validation", chip);
            return false;
        }
        entry += 6;
    }
    if (ReadU16BE(entry) != 0) {
        SetError(error, errorSize, "Cruis'n USA DCS catalog has no terminator");
        return false;
    }
    return true;
}

size_t FindSoftBootOffset(const std::vector<uint8_t>& u2)
{
    const auto isJump = [&u2](size_t offset) {
        return offset + 3 <= u2.size()
            && (u2[offset] & 0xfc) == 0x18
            && (u2[offset + 2] & 0x0f) == 0x0f;
    };
    if (isJump(0x1000))
        return 0x1000;
    if (isJump(0x2000))
        return 0x2000;
    return 0;
}

void ConfigureAutobuffer(int port, int32_t)
{
    if (gBoard == nullptr || port != 1)
        return;
    const uint16_t systemControl = gBoard->data[0x3fff];
    const uint16_t autobufferControl = gBoard->data[0x3fef];
    if ((systemControl & 0x0800) == 0 || (autobufferControl & 0x0002) == 0) {
        gBoard->autobuffer = false;
        return;
    }

    auto& regs = adsp2100_get_regs();
    gBoard->ireg = (autobufferControl >> 9) & 7;
    const int mreg = ((autobufferControl >> 7) & 3) | (gBoard->ireg & 4);
    gBoard->increment = regs.m[mreg];
    gBoard->size = regs.l[gBoard->ireg];
    gBoard->base = regs.i[gBoard->ireg] & ~0xf;
    regs.i[gBoard->ireg] = gBoard->base;
    gBoard->halfSamples = gBoard->increment == 0
        ? 0 : gBoard->size / (2 * gBoard->increment);
    gBoard->autobuffer = gBoard->halfSamples > 0;
    gBoard->cyclesUntilDma = gBoard->halfSamples * kCyclesPerSample;
}

void TransferHalfBuffer()
{
    if (gBoard == nullptr || !gBoard->autobuffer)
        return;
    auto& regs = adsp2100_get_regs();
    int read = regs.i[gBoard->ireg];
    for (int sample = 0; sample < gBoard->halfSamples; ++sample) {
        gBoard->output.push_back(static_cast<int16_t>(adsp2100_host_read_dm(read)));
        read += gBoard->increment;
    }
    const bool wrapped = read >= gBoard->base + gBoard->size;
    if (wrapped)
        read = gBoard->base;
    regs.i[gBoard->ireg] = read;
    if (wrapped) {
        adsp2105_set_irq_line(ADSP2105_IRQ1, 1);
        adsp2105_execute(1);
        adsp2105_set_irq_line(ADSP2105_IRQ1, 0);
    }
    gBoard->cyclesUntilDma = gBoard->halfSamples * kCyclesPerSample;
}

void RunCycles(int cycles)
{
    while (cycles > 0) {
        if (gBoard->autobuffer && gBoard->cyclesUntilDma <= 0)
            TransferHalfBuffer();
        const int slice = gBoard->autobuffer
            ? std::min(cycles, gBoard->cyclesUntilDma)
            : cycles;
        adsp2105_execute(slice);
        cycles -= slice;
        if (gBoard->autobuffer)
            gBoard->cyclesUntilDma -= slice;
    }
}

void SendByte(uint8_t value)
{
    for (int attempts = 0; gBoard->inputFull && attempts < 100; ++attempts)
        RunCycles(1000);
    gBoard->input = value;
    gBoard->inputFull = true;
    adsp2105_set_irq_line(ADSP2105_IRQ2, 1);
    for (int attempts = 0; gBoard->inputFull && attempts < 100; ++attempts)
        RunCycles(1000);
}

} // namespace

struct dcs_board {
    BoardState state;
};

uint32_t* adsp2100_op_rom = nullptr;

uint32_t adsp2100_host_read_dm(uint32_t address)
{
    address &= 0x3fff;
    if (address < 0x2000)
        return gBoard->program[0x0800 + (address & 0x07ff)] >> 8;
    if (address < 0x3000)
        return gBoard->sound[(gBoard->bank + address - 0x2000) % gBoard->sound.size()];
    if (address >= 0x3400 && address <= 0x37ff) {
        gBoard->inputFull = false;
        adsp2105_set_irq_line(ADSP2105_IRQ2, 0);
        return gBoard->input;
    }
    return gBoard->data[address];
}

void adsp2100_host_write_dm(uint32_t address, uint32_t value)
{
    address &= 0x3fff;
    if (address < 0x2000) {
        uint32_t& shared = gBoard->program[0x0800 + (address & 0x07ff)];
        shared = ((value & 0xffff) << 8) | (shared & 0xff);
    } else {
        gBoard->data[address] = static_cast<uint16_t>(value);
    }
    if (address >= 0x3000 && address <= 0x33ff)
        gBoard->bank = ((value & 0x07ff) * 0x1000) % gBoard->sound.size();
}

uint32_t adsp2100_host_read_pm(uint32_t address)
{
    return gBoard->program[address & 0x3fff];
}

void adsp2100_host_write_pm(uint32_t address, uint32_t value)
{
    gBoard->program[address & 0x3fff] = value;
}

extern "C" dcs_board* dcs_board_create(const char* zipPath, char* error, size_t errorSize)
{
    if (zipPath == nullptr || gBoard != nullptr) {
        SetError(error, errorSize, zipPath == nullptr
            ? "no DCS ROM zip path was supplied" : "a DCS board is already active");
        return nullptr;
    }
    std::array<std::vector<uint8_t>, kLastSoundChip + 1> roms;
    if (!LoadSoundROMs(zipPath, roms, error, errorSize))
        return nullptr;

    if (!ValidateSoundROMs(roms, error, errorSize))
        return nullptr;
    const size_t softBootOffset = FindSoftBootOffset(roms[kFirstSoundChip]);
    if (softBootOffset == 0) {
        SetError(error, errorSize, "Cruis'n USA DCS soft-boot image was not found");
        return nullptr;
    }

    dcs_board* board = new (std::nothrow) dcs_board;
    if (board == nullptr) {
        SetError(error, errorSize, "out of memory allocating the DCS board");
        return nullptr;
    }
    try {
        board->state.sound.assign(kSoundRegionWords, 0xffff);
        for (unsigned chip = kFirstSoundChip; chip <= kLastSoundChip; ++chip) {
            const size_t base = (chip - kFirstSoundChip) * kSocketSpacingWords;
            for (size_t offset = 0; offset < roms[chip].size(); ++offset)
                board->state.sound[base + offset] = 0xff00 | roms[chip][offset];
        }
    } catch (...) {
        delete board;
        SetError(error, errorSize, "out of memory loading the DCS ROM region");
        return nullptr;
    }

    gBoard = &board->state;
    adsp2100_op_rom = gBoard->program.data();
    adsp2105_init();
    adsp2105_reset(nullptr);
    adsp2105_set_tx_callback(ConfigureAutobuffer);
    // Start at the firmware's command-mode image. The arcade main board normally
    // reaches this through its power-on handshake, which the portable host omits.
    adsp2105_load_boot_data(
        roms[kFirstSoundChip].data() + softBootOffset,
        gBoard->program.data());
    for (int attempts = 0; !gBoard->autobuffer && attempts < 20000; ++attempts)
        RunCycles(1000);
    if (!gBoard->autobuffer) {
        dcs_board_destroy(board);
        SetError(error, errorSize, "DCS firmware did not start its audio stream");
        return nullptr;
    }
    return board;
}

extern "C" void dcs_board_destroy(dcs_board* board)
{
    if (board == nullptr)
        return;
    adsp2105_exit();
    adsp2100_op_rom = nullptr;
    gBoard = nullptr;
    delete board;
}

extern "C" void dcs_board_send_command(dcs_board* board, uint16_t command)
{
    if (board == nullptr || gBoard != &board->state)
        return;
    SendByte(static_cast<uint8_t>(command >> 8));
    SendByte(static_cast<uint8_t>(command));
}

extern "C" int16_t dcs_board_next_sample(dcs_board* board)
{
    if (board == nullptr || gBoard != &board->state)
        return 0;
    while (gBoard->output.empty()) {
        if (gBoard->autobuffer && gBoard->cyclesUntilDma <= 0)
            TransferHalfBuffer();
        else
            RunCycles(gBoard->autobuffer ? gBoard->cyclesUntilDma : kCyclesPerSample);
    }
    const int16_t sample = gBoard->output.front();
    gBoard->output.pop_front();
    return sample;
}
