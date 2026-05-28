#!/usr/bin/env python3
from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import Optional

END_OF_STREAM = 256
BUMP_CODE = 257
FLUSH_CODE = 258
FIRST_CODE = 259
BITS_MIN = 9
BITS_MAX = 12
MAX_CODE = (1 << BITS_MAX) - 1


@dataclass
class Config:
    rom: Path
    rom_base: int = 0xC10000
    addr_scale: int = 4
    word_endian: str = "little"  # little|big


def word_at(data: bytes, cfg: Config, addr_word: int) -> int:
    off = (addr_word - cfg.rom_base) * cfg.addr_scale
    if off < 0 or off + 4 > len(data):
        raise IndexError
    return int.from_bytes(data[off:off + 4], cfg.word_endian)


def input_bits(data: bytes, cfg: Config, ar0: int, bit_addr: int, nbits: int):
    r0 = bit_addr + nbits
    if r0 <= 31:
        w = word_at(data, cfg, ar0)
        # mirror ASM: LSH BIT_ADDR,R0 then LSH (nbits-32),R0
        v = (w << bit_addr) & 0xFFFFFFFF
        v = (v >> (32 - nbits)) & ((1 << nbits) - 1)
        bit_addr += nbits
        return v, ar0, bit_addr

    w1 = word_at(data, cfg, ar0)
    ar0 += 1
    # left justify first word
    part1 = (w1 << bit_addr) & 0xFFFFFFFF

    bits_in_first = 32 - bit_addr
    bits_in_second = nbits - bits_in_first
    bit_addr = bits_in_second

    w2 = word_at(data, cfg, ar0)
    part2 = (w2 >> (32 - bits_in_second)) & ((1 << bits_in_second) - 1)

    val = ((part1 >> (32 - nbits)) | part2) & ((1 << nbits) - 1)
    return val, ar0, bit_addr


def find_compressed_length(data: bytes, cfg: Config, src_addr: int, max_steps: int = 10_000_000) -> Optional[int]:
    ar0 = src_addr
    bit_addr = 0

    current_bits = BITS_MIN
    next_code = FIRST_CODE

    try:
        old_code, ar0, bit_addr = input_bits(data, cfg, ar0, bit_addr, current_bits)
    except IndexError:
        return None

    if old_code == END_OF_STREAM:
        return 0

    # emulate dictionary growth enough to reject bad streams
    for _ in range(max_steps):
        try:
            new_code, ar0, bit_addr = input_bits(data, cfg, ar0, bit_addr, current_bits)
        except IndexError:
            return None

        if new_code == END_OF_STREAM:
            # bytes consumed from src_addr to current (ar0,bit_addr)
            words = ar0 - src_addr
            if bit_addr > 0:
                words += 1
            return words * 4

        if new_code == FLUSH_CODE:
            current_bits = BITS_MIN
            next_code = FIRST_CODE
            try:
                old_code, ar0, bit_addr = input_bits(data, cfg, ar0, bit_addr, current_bits)
            except IndexError:
                return None
            if old_code == END_OF_STREAM:
                words = ar0 - src_addr
                if bit_addr > 0:
                    words += 1
                return words * 4
            continue

        if new_code == BUMP_CODE:
            if current_bits < BITS_MAX:
                current_bits += 1
            continue

        # basic sanity: codes should not run wildly ahead
        if new_code > next_code + 1 or new_code > MAX_CODE:
            return None

        if next_code <= MAX_CODE:
            next_code += 1
        old_code = new_code

    return None


def main():
    roms = [
        Path('/Users/j.harris/code/cruisin-usa/roms/crusnusa41_maindata_linear.bin'),
        Path('/Users/j.harris/code/cruisin-usa/roms/crusnusa41_maindata_linear_bswap32.bin'),
    ]
    srcs = {
        'SECcorn': 0x00C16800,
        'SEChead2head': 0x00C15000,
    }

    for rom in roms:
        data = rom.read_bytes()
        print(f'ROM: {rom.name}')
        for scale in (1, 4):
            for endian in ('little', 'big'):
                cfg = Config(rom=rom, addr_scale=scale, word_endian=endian)
                vals = {}
                ok = True
                for name, src in srcs.items():
                    ln = find_compressed_length(data, cfg, src)
                    vals[name] = ln
                    if ln is None or ln < 64:
                        ok = False
                status = 'LIKELY' if ok else 'no'
                print(f'  scale={scale} endian={endian} -> {vals} [{status}]')

if __name__ == '__main__':
    main()
