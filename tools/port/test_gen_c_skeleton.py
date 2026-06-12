#!/usr/bin/env python3

import tempfile
import unittest
from pathlib import Path

from gen_c_skeleton import (
    DefineEntry,
    StorageVariable,
    collect_defined_data_symbols,
    collect_referenced_define_symbols,
    collect_referenced_data_symbols,
    parse_discovered_defines_file,
    render_storage_header,
    render_discovered_defines_header,
    render_module,
)


class GenCSkeletonTests(unittest.TestCase):
    def test_sptr_labels_render_as_top_level_strings(self) -> None:
        asm_source = """MSG2:\tLDI\t11,RC
\tTEXTIT\tM6,1,260
\tTEXTIT\tM7,1,270
\tRETS

LINKDISABLED\tSPTR\t"LINK DISABLED BY U97  DIP6 OFF"
IAMMASTER\tSPTR\t"LINK MASTER MACHINE"

MSG3:\tLDI\t11,RC
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "CUSA.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn('void MSG2(void)\n{\n    // asm: LDI\t11,RC', rendered)
        self.assertIn('const char *LINKDISABLED = "LINK DISABLED BY U97  DIP6 OFF";', rendered)
        self.assertIn('const char *IAMMASTER = "LINK MASTER MACHINE";', rendered)
        self.assertIn('void MSG3(void)', rendered)
        self.assertNotIn('LINKDISABLED:\n    // asm: SPTR\t"LINK DISABLED BY U97  DIP6 OFF"', rendered)
        self.assertNotIn('\nchar *LINKDISABLED = "LINK DISABLED BY U97  DIP6 OFF";', rendered)
        self.assertEqual(rendered.count("// asm: LDI\t11,RC"), 2)

    def test_standalone_label_word_renders_as_data(self) -> None:
        asm_source = """\
    .globl\t_SEChead2head
_SEChead2head:\t\t;(16345 lines, 102.16%)
\t.word\t0C15000h
\t.word\t0BEFA00h
"""
        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "OBJECTS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("int _SEChead2head[] = {\n    0x0C15000,\n    0x0BEFA00,\n};", rendered)
        self.assertNotIn("void _SEChead2head(void)", rendered)

    def test_single_symbol_word_renders_as_define(self) -> None:
        asm_source = """CREDITBUFFI\t.word\tCREDITBUFFER\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "COIN.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("/* asm: CREDITBUFFI\t.word\tCREDITBUFFER */", rendered)
        self.assertIn("#define CREDITBUFFI CREDITBUFFER", rendered)
        self.assertNotIn("int CREDITBUFFI = (int)(CREDITBUFFER);", rendered)

    def test_single_symbol_word_lowercase_i_suffix_renders_as_define(self) -> None:
        asm_source = """creditbuffi\t.word\tCREDITBUFFER\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "COIN.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("/* asm: creditbuffi\t.word\tCREDITBUFFER */", rendered)
        self.assertIn("#define creditbuffi CREDITBUFFER", rendered)
        self.assertNotIn("int creditbuffi = (int)(CREDITBUFFER);", rendered)

    def test_single_symbol_word_without_i_suffix_stays_int(self) -> None:
        asm_source = """CREDITBUFF\t.word\tCREDITBUFFER\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "COIN.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("/* asm: CREDITBUFF\t.word\tCREDITBUFFER */", rendered)
        self.assertIn("int CREDITBUFF = CREDITBUFFER;", rendered)
        self.assertNotIn("#define CREDITBUFF CREDITBUFFER", rendered)

    def test_single_symbol_word_pointing_at_following_label_renders_as_define(self) -> None:
        asm_source = """INFINITY_POINTS\t.word\tINFINPOINTS\nINFINPOINTS\n\t.float\t-1280,0,0\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "INFIN.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("/* asm: INFINITY_POINTS\t.word\tINFINPOINTS */", rendered)
        self.assertIn("#define INFINITY_POINTS INFINPOINTS", rendered)
        self.assertNotIn("int INFINITY_POINTS = INFINPOINTS;", rendered)

    def test_collects_skipped_data_symbol_references(self) -> None:
        asm_lines = [
            "\tLDL\t_SECshared,AR2",
            "\tLDL\t_SECshared_ptr_0000B0BF,AR2",
            "\tCALL\tSOMEFUNC",
        ]
        label_types = {
            "_SECshared": "data",
            "_SECshared_ptr_0000B0BF": "data",
            "SOMEFUNC": "code",
        }

        self.assertEqual(collect_defined_data_symbols(asm_lines), set())
        self.assertEqual(collect_referenced_data_symbols(asm_lines, label_types), {"_SECshared"})

    def test_parse_discovered_defines_file(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            defines_path = Path(tmpdir) / "discovered_defines.txt"
            defines_path.write_text("cam_left_stop\t-4700\tINTRO\nbottom_gtmp_p\t0\tINTRO\n")

            parsed = parse_discovered_defines_file(defines_path)

        self.assertEqual(parsed["cam_left_stop"].expr, "-4700")
        self.assertEqual(parsed["cam_left_stop"].module, "INTRO")
        self.assertEqual(parsed["bottom_gtmp_p"].expr, "0")

    def test_render_module_includes_discovered_header_when_needed(self) -> None:
        asm_source = """INTRO:\tLDI\tbottom_gtmp_p,R0\n\tRETS\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "INTRO.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(
                src_path,
                {},
                {},
                None,
                True,
            )

        self.assertIn('#include "../include/discovered_defines.h"', rendered)
        self.assertNotIn("#define bottom_gtmp_p", rendered)

    def test_render_discovered_defines_header_uses_decimal_values(self) -> None:
        rendered = render_discovered_defines_header([
            DefineEntry(name="bottom_gtmp_p", expr="0", module="INTRO"),
            DefineEntry(name="cam_left_stop", expr="-4700", module="INTRO"),
        ])

        self.assertIn("#define bottom_gtmp_p 0", rendered)
        self.assertIn("#define cam_left_stop (-4700)", rendered)

    def test_render_discovered_defines_header_keeps_uppercase_i_suffix_names(self) -> None:
        rendered = render_discovered_defines_header([
            DefineEntry(name="sky1_I", expr="854", module="INFIN"),
            DefineEntry(name="sky2_I", expr="1110", module="INFIN"),
        ])

        self.assertIn("#define sky1_I 854", rendered)
        self.assertIn("#define sky2_I 1110", rendered)

    def test_collect_referenced_define_symbols(self) -> None:
        asm_lines = [
            "\tLDI\tbottom_gtmp_p,R0",
            "\tADDI\tcam_left_stop,R1",
            "\tCALL\tSOMEFUNC",
        ]

        refs = collect_referenced_define_symbols(asm_lines, {"bottom_gtmp_p", "cam_left_stop"})

        self.assertEqual(refs, {"bottom_gtmp_p", "cam_left_stop"})

    def test_render_storage_header_omits_address_comments(self) -> None:
        rendered = render_storage_header(
            Path("HEADS.ASM"),
            [
                StorageVariable(
                    name="heads_count",
                    addr=0x0000E62A,
                    module="HEADS",
                    asm_line=".bss heads_count,1",
                    size_expr="1",
                )
            ],
        )

        self.assertIn("// asm: .bss heads_count,1", rendered)
        self.assertIn("extern int heads_count;", rendered)
        self.assertNotIn("// addr:", rendered)

    def test_equ_file_renders_comment_header_and_set_defines(self) -> None:
        asm_source = """*DELTA.EQU
*
*COPYRIGHT (C) 1994 BY  TV GAMES, INC.
*ALL RIGHTS RESERVED

DELTA_SAFETYWIDTH\t.set\t850
MAX_DRONES\t.set\t11\t;MAXIMUM DRONES IN UNIVERSE
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "DELTA.EQU"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("// DELTA.EQU", rendered)
        self.assertIn("// ", rendered)
        self.assertIn("// COPYRIGHT (C) 1994 BY  TV GAMES, INC.", rendered)
        self.assertIn("// ALL RIGHTS RESERVED", rendered)
        self.assertIn("// asm: DELTA_SAFETYWIDTH\t.set\t850", rendered)
        self.assertIn("#define DELTA_SAFETYWIDTH 850", rendered)
        self.assertIn("// asm: MAX_DRONES\t.set\t11\t;MAXIMUM DRONES IN UNIVERSE", rendered)
        self.assertIn("#define MAX_DRONES 11 //MAXIMUM DRONES IN UNIVERSE", rendered)
        self.assertNotIn("void delta(void)", rendered)


if __name__ == "__main__":
    unittest.main()
