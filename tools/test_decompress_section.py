#!/usr/bin/env python3

import tempfile
import unittest
from pathlib import Path

from decompress_section import (
    DEFAULT_MAP,
    DEFAULT_ROM,
    decompress_section_bytes,
    parse_address_map,
    read_descriptor,
)


class DecompressSectionTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.rom_bytes = DEFAULT_ROM.read_bytes()
        cls.address_map = parse_address_map(DEFAULT_MAP)

    def test_sec_corn_matches_expected_length(self) -> None:
        descriptor = read_descriptor(self.rom_bytes, self.address_map["_SECcorn"])
        output = decompress_section_bytes(self.rom_bytes, descriptor.source_addr)
        self.assertEqual(descriptor.source_addr, 0x00C16800)
        self.assertEqual(len(output), 30464)

    def test_sec_head2head_matches_expected_length(self) -> None:
        descriptor = read_descriptor(self.rom_bytes, self.address_map["_SEChead2head"])
        output = decompress_section_bytes(self.rom_bytes, descriptor.source_addr)
        self.assertEqual(descriptor.source_addr, 0x00C15000)
        self.assertEqual(len(output), 124160)


if __name__ == "__main__":
    unittest.main()
