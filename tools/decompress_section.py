#!/usr/bin/env python3
"""Decompress a section descriptor from the Cruis'n USA word ROM."""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path


ROM_WINDOW_BASE = 0x00C00000
END_OF_STREAM = 256
BUMP_CODE = 257
FLUSH_CODE = 258
FIRST_CODE = 259
BITS_MIN = 9
BITS_MAX = 12
MAX_CODE = (1 << BITS_MAX) - 1
ROOT = Path(__file__).resolve().parents[1]
DEFAULT_ROM = ROOT / "roms" / "crusnusa45_maindata_interleaved_bswap32.bin"
DEFAULT_MAP = ROOT / "tools" / "ida" / "address.map"


@dataclass
class SectionDescriptor:
    descriptor_addr: int
    source_addr: int
    dest_addr: int


def parse_int(value: str) -> int:
    text = value.strip()
    return int(text, 0)


def parse_address_map(map_path: Path) -> dict[str, int]:
    out: dict[str, int] = {}
    if not map_path.exists():
        return out
    for raw in map_path.read_text(errors="ignore").splitlines():
        line = raw.strip()
        if not line or line.startswith(";"):
            continue
        parts = line.split()
        if len(parts) < 2 or ":" not in parts[0]:
            continue
        _seg, off = parts[0].split(":", 1)
        try:
            out[parts[1]] = int(off, 16)
        except ValueError:
            continue
    return out


def resolve_descriptor(value: str, map_path: Path) -> int:
    try:
        return parse_int(value)
    except ValueError:
        symbol_map = parse_address_map(map_path)
        if value not in symbol_map:
            raise SystemExit(f"unknown descriptor symbol: {value}")
        return symbol_map[value]


def map_word_address_to_linear(word_addr: int) -> int:
    if ROM_WINDOW_BASE <= word_addr:
        return word_addr - ROM_WINDOW_BASE
    return word_addr


def read_rom_word(data: bytes, word_addr: int) -> int:
    linear = map_word_address_to_linear(word_addr)
    off = linear * 4
    if linear < 0 or off + 4 > len(data):
        raise ValueError(f"ROM read out of range at word address 0x{word_addr:08X}")
    return int.from_bytes(data[off:off + 4], "big")


def read_descriptor(data: bytes, descriptor_addr: int) -> SectionDescriptor:
    return SectionDescriptor(
        descriptor_addr=descriptor_addr,
        source_addr=read_rom_word(data, descriptor_addr),
        dest_addr=read_rom_word(data, descriptor_addr + 1),
    )


def input_bits(data: bytes, word_addr: int, bit_addr: int, nbits: int) -> tuple[int, int, int]:
    next_bit = bit_addr + nbits
    if next_bit <= 31:
        word = read_rom_word(data, word_addr)
        value = (word << bit_addr) & 0xFFFFFFFF
        value = (value >> (32 - nbits)) & ((1 << nbits) - 1)
        return value, word_addr, bit_addr + nbits

    first_word = read_rom_word(data, word_addr)
    word_addr += 1
    part1 = (first_word << bit_addr) & 0xFFFFFFFF
    bits_in_first = 32 - bit_addr
    bits_in_second = nbits - bits_in_first
    bit_addr = bits_in_second

    second_word = read_rom_word(data, word_addr)
    part2 = (second_word >> (32 - bits_in_second)) & ((1 << bits_in_second) - 1)
    value = ((part1 >> (32 - nbits)) | part2) & ((1 << nbits) - 1)
    return value, word_addr, bit_addr


def decompress_section_raw_bytes(data: bytes, source_addr: int, max_steps: int = 10_000_000) -> bytes:
    word_addr = source_addr
    bit_addr = 0
    current_bits = BITS_MIN
    next_code = FIRST_CODE
    parents = [-1] * (MAX_CODE + 1)
    characters = [0] * (MAX_CODE + 1)
    out = bytearray()

    old_code, word_addr, bit_addr = input_bits(data, word_addr, bit_addr, current_bits)
    if old_code == END_OF_STREAM:
        return bytes(out)
    if old_code > 0xFF:
        raise ValueError(f"invalid initial code 0x{old_code:X}")

    character = old_code
    out.append(character)

    for _ in range(max_steps):
        new_code, word_addr, bit_addr = input_bits(data, word_addr, bit_addr, current_bits)
        if new_code == END_OF_STREAM:
            return bytes(out)
        if new_code == FLUSH_CODE:
            current_bits = BITS_MIN
            next_code = FIRST_CODE
            parents = [-1] * (MAX_CODE + 1)
            characters = [0] * (MAX_CODE + 1)
            old_code, word_addr, bit_addr = input_bits(data, word_addr, bit_addr, current_bits)
            if old_code == END_OF_STREAM:
                return bytes(out)
            if old_code > 0xFF:
                raise ValueError(f"invalid code after flush 0x{old_code:X}")
            character = old_code
            out.append(character)
            continue
        if new_code == BUMP_CODE:
            if current_bits >= BITS_MAX:
                raise ValueError("bit-width bump past 12 bits")
            current_bits += 1
            continue

        if new_code >= next_code:
            if new_code > MAX_CODE:
                raise ValueError(f"invalid code 0x{new_code:X} (next_code=0x{next_code:X})")
            stack = [character]
            code = old_code
        else:
            stack: list[int] = []
            code = new_code

        while code > 0xFF:
            if code >= len(parents) or parents[code] < 0:
                raise ValueError(f"dictionary parent missing for code 0x{code:X}")
            stack.append(characters[code])
            code = parents[code]

        character = code & 0xFF
        out.append(character)
        while stack:
            out.append(stack.pop())

        if next_code <= MAX_CODE:
            parents[next_code] = old_code
            characters[next_code] = character
            next_code += 1
        old_code = new_code

    raise ValueError("decompression did not terminate before max_steps")


def pack_waveram_words(raw_bytes: bytes) -> bytes:
    out = bytearray()
    for offset in range(0, len(raw_bytes), 4):
        chunk = raw_bytes[offset:offset + 4]
        chunk += b"\x00" * (4 - len(chunk))
        packed_word = int.from_bytes(chunk, "little")
        out += (packed_word & 0xFFFF).to_bytes(4, "little")
        out += ((packed_word >> 16) & 0xFFFF).to_bytes(4, "little")
    return bytes(out)


def decompress_section_bytes(data: bytes, source_addr: int, max_steps: int = 10_000_000) -> bytes:
    return pack_waveram_words(decompress_section_raw_bytes(data, source_addr, max_steps=max_steps))


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("descriptor", help="section descriptor word address or symbol from address.map")
    parser.add_argument("output", type=Path, help="output file for decompressed bytes")
    parser.add_argument("--rom", type=Path, default=DEFAULT_ROM, help=f"word ROM file (default: {DEFAULT_ROM})")
    parser.add_argument("--address-map", type=Path, default=DEFAULT_MAP, help=f"address map (default: {DEFAULT_MAP})")
    return parser


def main() -> int:
    args = build_arg_parser().parse_args()
    descriptor_addr = resolve_descriptor(args.descriptor, args.address_map)
    rom_bytes = args.rom.read_bytes()
    descriptor = read_descriptor(rom_bytes, descriptor_addr)
    output = decompress_section_bytes(rom_bytes, descriptor.source_addr)
    args.output.write_bytes(output)
    print(
        f"descriptor=0x{descriptor.descriptor_addr:08X} "
        f"src=0x{descriptor.source_addr:08X} "
        f"dst=0x{descriptor.dest_addr:08X} "
        f"bytes={len(output)} "
        f"output={args.output}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
