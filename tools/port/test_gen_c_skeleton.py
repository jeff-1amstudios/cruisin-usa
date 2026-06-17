#!/usr/bin/env python3

import re
import tempfile
import unittest
from pathlib import Path

from gen_equ_headers import parse_equ_file, render_header
from gen_c_skeleton import (
    DefineEntry,
    LabelEntry,
    StorageVariable,
    TypeOverride,
    collect_existing_macro_names,
    collect_source_label_names,
    collect_defined_data_symbols,
    collect_module_symbol_table,
    collect_referenced_define_symbols,
    collect_referenced_data_symbols,
    parse_discovered_defines_file,
    parse_instruction_addresses_file,
    parse_render_overrides_file,
    parse_type_overrides_file,
    merge_storage_into_header,
    render_discovered_defines_header,
    render_discovered_labels_header,
    render_port_header,
    render_storage_header,
    render_module,
    variable_declaration,
)


class GenCSkeletonTests(unittest.TestCase):
    def extract_function_body(self, rendered: str, name: str) -> str:
        match = re.search(rf"void {re.escape(name)}\(void\)\n\{{\n(.*?)\n\}}", rendered, re.S)
        self.assertIsNotNone(match)
        return match.group(1)

    def test_variable_declaration_omits_size_for_extern_arrays(self) -> None:
        self.assertEqual(variable_declaration("OBJSTR", "int", "OBJSIZ*NUM_OBJECTS", is_extern=True), "extern int OBJSTR[];")
        self.assertEqual(variable_declaration("TABLE", "float", "16", is_extern=True), "extern float TABLE[];")

    def test_render_module_preserves_interleaved_top_level_order(self) -> None:
        asm_source = """\t.bss\tVAR1,1
FUNC1:\tRETS
\t.bss\tVAR2,1
FUNC2:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "ORDER.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        var1_idx = rendered.index("int VAR1;")
        func1_idx = rendered.index("void FUNC1(void)\n{")
        var2_idx = rendered.index("int VAR2;")
        func2_idx = rendered.index("void FUNC2(void)\n{")
        self.assertLess(var1_idx, func1_idx)
        self.assertLess(func1_idx, var2_idx)
        self.assertLess(var2_idx, func2_idx)

    def test_merge_storage_into_equ_header_reuses_same_header(self) -> None:
        existing = """#ifndef OBJ_H
#define OBJ_H

/* Generated from asm/OBJ.EQU. */

#define NUM_OBJECTS 1100

#endif /* OBJ_H */
"""
        merged = merge_storage_into_header(
            existing,
            Path("OBJ.ASM"),
            [StorageVariable("OACTIVE", 0, "OBJ", "\t.bss\tOACTIVE,1", "1")],
        )

        self.assertIn("/* Generated from asm/OBJ.EQU. */", merged)
        self.assertIn("\n// OBJ.EQU\n#define NUM_OBJECTS 1100\n\n// OBJ.ASM\n", merged)
        self.assertIn("extern int OACTIVE;", merged)
        self.assertEqual(merged.count("// OBJ.ASM"), 1)

    def test_merge_storage_into_equ_header_skips_duplicate_declarations(self) -> None:
        existing = """#ifndef OBJ_H
#define OBJ_H

/* Generated from asm/OBJ.EQU. */

// OBJ.EQU
extern int OACTIVE;
extern int OBJSTR[OBJSIZ*NUM_OBJECTS];
void OBJ_INIT(void);

#endif /* OBJ_H */
"""
        merged = merge_storage_into_header(
            existing,
            Path("OBJ.ASM"),
            [
                StorageVariable("OACTIVE", 0, "OBJ", "\t.bss\tOACTIVE,1", "1"),
                StorageVariable("OBJSTR", 0, "OBJ", "\t.hibss\tOBJSTR,OBJSIZ*NUM_OBJECTS", "OBJSIZ*NUM_OBJECTS"),
                StorageVariable("COMM_DRONE_PTR", 0, "OBJ", "\t.bss\tCOMM_DRONE_PTR,1", "1"),
            ],
        )

        self.assertEqual(merged.count("extern int OACTIVE;"), 1)
        self.assertEqual(merged.count("extern int OBJSTR[OBJSIZ*NUM_OBJECTS];"), 1)
        self.assertIn("// OBJ.ASM", merged)
        self.assertIn("extern int COMM_DRONE_PTR;", merged)

    def test_merge_storage_into_equ_header_is_idempotent(self) -> None:
        existing = """#ifndef OBJ_H
#define OBJ_H

/* Generated from asm/OBJ.EQU. */

// OBJ.EQU
#define NUM_OBJECTS 1100

// OBJ.ASM
// asm: \t.bss\tOACTIVE,1
extern int OACTIVE;

#endif /* OBJ_H */
"""
        merged = merge_storage_into_header(
            existing,
            Path("OBJ.ASM"),
            [StorageVariable("OACTIVE", 0, "OBJ", "\t.bss\tOACTIVE,1", "1")],
        )

        self.assertEqual(merged.count("// OBJ.EQU"), 1)
        self.assertEqual(merged.count("// OBJ.ASM"), 1)
        self.assertEqual(merged.count("extern int OACTIVE;"), 1)

    def test_parse_instruction_addresses_file(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            addresses_path = Path(tmpdir) / "romlst_instruction_addresses.tsv"
            addresses_path.write_text("module\tsource_line\taddress\nCUSA\t12\t00004AE6\nCUSA\t13\t00004AE9\n")

            parsed = parse_instruction_addresses_file(addresses_path)

        self.assertEqual(parsed["CUSA"][12], 0x00004AE6)
        self.assertEqual(parsed["CUSA"][13], 0x00004AE9)

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

    def test_function_comments_include_instruction_addresses_when_available(self) -> None:
        asm_source = """START:\tLDI\t11,RC
\tTEXTIT\tM6,1,260
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "CUSA.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(
                src_path,
                {},
                {},
                None,
                False,
                None,
                False,
                {1: 0x00004AE6, 2: 0x00004AE9, 3: 0x00004AED},
            )

        self.assertIn("    // asm 00004AE6: LDI\t11,RC", rendered)
        self.assertIn("    // asm 00004AE9: \tTEXTIT\tM6,1,260", rendered)
        self.assertIn("    // asm 00004AED: \tRETS", rendered)
        self.assertNotIn("    // asm: \tTEXTIT\tM6,1,260", rendered)

    def test_label_only_function_line_uses_next_instruction_address(self) -> None:
        asm_source = """START:\tLDI\t11,RC
NEXTLBL
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "CUSA.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(
                src_path,
                {},
                {},
                None,
                False,
                None,
                False,
                {1: 0x00004AE6, 3: 0x00004AE9},
            )

        self.assertIn("    // asm 00004AE9: NEXTLBL", rendered)

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

    def test_float_data_and_word_pointer_table_render_with_float_types(self) -> None:
        asm_source = """LANEP\t.word\tLANES,LANES4
LANES\t.float\t-576.0,-576.0,576.0,576.0
LANES4\t.float\t-1728.0,-576.0,576.0,1728.0
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "DRONES.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("float *LANEP[] = {\n    LANES, LANES4,\n};", rendered)
        self.assertIn("float LANES[] = {\n    -576.0f, -576.0f, 576.0f, 576.0f,\n};", rendered)
        self.assertIn("float LANES4[] = {\n    -1728.0f, -576.0f, 576.0f, 1728.0f,\n};", rendered)

    def test_string_label_table_collapses_unreferenced_children_to_literals(self) -> None:
        asm_source = """LEG_NAMES\t.word\tLEG1,LEG2
LEG1\t.string\t"GOLDEN GATE PARK",0
LEG2\t.string\t"SAN FRANCISCO",0
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn('const char *LEG_NAMES[] = { "GOLDEN GATE PARK", "SAN FRANCISCO" };', rendered)
        self.assertNotIn("const char * *LEG_NAMES[]", rendered)
        self.assertNotIn('const char *LEG1 = "GOLDEN GATE PARK";', rendered)
        self.assertNotIn('const char *LEG2 = "SAN FRANCISCO";', rendered)

    def test_string_label_table_keeps_children_when_directly_referenced(self) -> None:
        asm_source = """LEG_NAMES\t.word\tLEG1,LEG2
\tTEXTIT\tLEG1,1,100
LEG1\t.string\t"GOLDEN GATE PARK",0
LEG2\t.string\t"SAN FRANCISCO",0
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("const char *LEG_NAMES[] = {\n    LEG1, LEG2,\n};", rendered)
        self.assertIn('const char *LEG1 = "GOLDEN GATE PARK";', rendered)
        self.assertIn('const char *LEG2 = "SAN FRANCISCO";', rendered)

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

    def test_branch_target_label_is_not_misclassified_as_standalone_data(self) -> None:
        asm_source = """\tBZ\tCONTINUE
CONTINUE

\t.bss\tWAS_HEAD2HEAD_ON,1
"""

        asm_lines = asm_source.splitlines()
        self.assertEqual(collect_defined_data_symbols(asm_lines), {"WAS_HEAD2HEAD_ON"})

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertNotIn("int CONTINUE[WAS_HEAD2HEAD_ON];", rendered)
        self.assertIn("int WAS_HEAD2HEAD_ON;", rendered)

    def test_data_only_macro_after_bare_label_closes_previous_function(self) -> None:
        asm_source = """AUDENT .MACRO A
\t.word\t:A:
\t.string\t"A",0
\t.ENDM

COMPUTE_GAMETIME:\tLDI\tR0,R1
\tRETS
AUDIT_LIST
\tAUDENT\tENTRY1

NEXTFUNC:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "AUDITS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void COMPUTE_GAMETIME(void)", rendered)
        self.assertIn("    // asm: \tRETS", rendered)
        self.assertIn("/* asm: AUDIT_LIST */", rendered)
        self.assertIn("/* asm: AUDENT\tENTRY1 */", rendered)
        self.assertIn("int AUDIT_LIST;", rendered)
        self.assertIn("void NEXTFUNC(void)", rendered)
        compute_section = rendered.split("void COMPUTE_GAMETIME(void)", 1)[1].split("void NEXTFUNC(void)", 1)[0]
        self.assertNotIn("AUDIT_LIST", compute_section)

    def test_bare_inline_branch_label_does_not_start_function(self) -> None:
        asm_source = """AUDIT_DISPLAY:\tCALL\tSETUP_DIAG_SCREEN
\tBNE\tNOCALL
\tLDI\t*AR5++,R0
\tCALLU\tR0
\tBU\tJ22AB

NOCALL\tCALL\tAUDIT_READ
J22AB\tLDI\tR0,R2
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "AUDITS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void AUDIT_DISPLAY(void)", rendered)
        self.assertNotIn("void NOCALL(void)", rendered)
        audit_section = rendered.split("void AUDIT_DISPLAY(void)", 1)[1]
        self.assertIn("NOCALL:", audit_section)
        self.assertIn("J22AB:", audit_section)

    def test_conditional_executable_macro_does_not_truncate_function(self) -> None:
        asm_source = """DEBUG\t.set\t0
SLOCKON .MACRO UCASE,UTEXT
\t.if\tDEBUG
\tB:UCASE:\t$
\t.endif
\t.ENDM

BGD_WATCHER:\tLDI\t@DGROUP_COUNT,RC
\tSUBI\t2,RC
\tSLOCKON\tLT,\"BACKGRND\\\\LBACK_WATCH ERROR\"
\tBLT\tSHIFT1
SHIFT1:\tDECM\t@DGROUP_COUNT
\tRETS

NEXTFUNC:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BACKGRND.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void BGD_WATCHER(void)", rendered)
        self.assertIn('    // asm: \tSLOCKON\tLT,"BACKGRND\\\\LBACK_WATCH ERROR"', rendered)
        self.assertIn("SHIFT1:", rendered)
        self.assertIn("void NEXTFUNC(void)", rendered)
        watcher_section = rendered.split("void BGD_WATCHER(void)\n{", 1)[1].split("\nvoid NEXTFUNC(void)\n{", 1)[0]
        self.assertIn("    // asm: \tBLT\tSHIFT1", watcher_section)
        self.assertIn("SHIFT1:", watcher_section)

    def test_orphan_top_level_instruction_raises(self) -> None:
        asm_source = """FUNC:\tRETS

BROKEN_TABLE
\t.word\t1
\tBLT\tSHIFT1
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BROKEN.ASM"
            src_path.write_text(asm_source)
            with self.assertRaisesRegex(ValueError, r"unexpected top-level code at line 5"):
                render_module(src_path, {}, {}, None)

    def test_function_local_data_island_stays_inside_function(self) -> None:
        asm_source = """BGD_ACTIVATE_TYCOGROUP:\tLDI\t32600,RC
\t.data
LVAL\t.word\t151720
\t.text
\tADDI\t@LVAL,RC
\tRETS

NEXTFUNC:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BACKGRND.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void BGD_ACTIVATE_TYCOGROUP(void)", rendered)
        section = rendered.split("void BGD_ACTIVATE_TYCOGROUP(void)\n{", 1)[1].split("\nvoid NEXTFUNC(void)\n{", 1)[0]
        self.assertIn("    // asm: \tADDI\t@LVAL,RC", section)
        self.assertNotIn("int LVAL", rendered)

    def test_branch_target_after_local_data_keeps_function_open(self) -> None:
        asm_source = """SETPAGE1:\tLDI\t@PAGEWORD,R0
\tBNE\tP1
\tRETS
PAGEWORD\t.word\t0
P1
\tLDI\t@SCREEN1I,R0
\tRETS

NEXTFUNC:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "UTIL.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void SETPAGE1(void)", rendered)
        section = rendered.split("void SETPAGE1(void)\n{", 1)[1].split("\nvoid NEXTFUNC(void)\n{", 1)[0]
        self.assertIn("P1:", section)
        self.assertIn("    // asm: \tLDI\t@SCREEN1I,R0", section)
        self.assertNotIn("int PAGEWORD", rendered)

    def test_parse_discovered_defines_file(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            defines_path = Path(tmpdir) / "discovered_defines.txt"
            defines_path.write_text("cam_left_stop\t-4700\tINTRO\nbottom_gtmp_p\t0\tINTRO\n")

            parsed = parse_discovered_defines_file(defines_path)

        self.assertEqual(parsed["cam_left_stop"].expr, "-4700")
        self.assertEqual(parsed["cam_left_stop"].module, "INTRO")
        self.assertEqual(parsed["bottom_gtmp_p"].expr, "0")

    def test_render_module_includes_port_header_when_discovered_header_is_needed(self) -> None:
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

        self.assertIn('#include "port.h"', rendered)
        self.assertNotIn('#include "discovered_defines.h"', rendered)
        self.assertNotIn("#define bottom_gtmp_p", rendered)

    def test_render_module_includes_port_header_when_discovered_labels_are_needed(self) -> None:
        asm_source = """SPIN_CARTAB\t.word\tmissle,hotrod\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "ATTRACTA.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(
                src_path,
                {"missle": 0x00C28DF4, "hotrod": 0x00C28A00},
                {},
                None,
                False,
                None,
                True,
            )

        self.assertIn('#include "port.h"', rendered)
        self.assertNotIn('#include "discovered_labels.h"', rendered)
        self.assertNotIn("#define missle", rendered)

    def test_render_port_header(self) -> None:
        rendered = render_port_header()

        self.assertIn('#include "discovered_defines.h"', rendered)
        self.assertIn('#include "discovered_labels.h"', rendered)
        self.assertIn('#include "types.h"', rendered)

    def test_label_followed_by_if_stays_function_not_top_level_data(self) -> None:
        asm_source = """VERIFY_CODE_INTEGRITY:\n\t.if\tDEBUG\n\tBNE\t$\n\t.endif\n\tRETS\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "CUSA.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void VERIFY_CODE_INTEGRITY(void)", rendered)
        self.assertIn("#if DEBUG", rendered)
        self.assertIn("    // asm: \tBNE\t$", rendered)
        self.assertNotIn("\n#endif\n#endif\n", rendered)

    def test_macro_definition_body_is_ignored(self) -> None:
        asm_source = """AUDENT\t.MACRO\tAUDITI,ATEXT
\t.word\t:AUDITI:,l?
\t.sect\t"THEDATA"
l?\t.string\t":ATEXT:",0
\t.text
\t.ENDM

OLDDIP\t.bss\tOLDDIP,1
AUDIT_DISPLAY:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "AUDITS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("OLDDIP", rendered)
        self.assertIn("void AUDIT_DISPLAY(void)", rendered)
        self.assertNotIn('const char *l_ = ":ATEXT:";', rendered)
        self.assertNotIn("void _word(void)", rendered)

    def test_struct_definition_body_is_ignored_in_module_render(self) -> None:
        asm_source = """VISIBLE\t.set\t1
*STRUCT\tTEST
FIELD0\t.set\t0
FIELD1\t.set\t1
*ENDSTRUCT
AFTER\t.set\t2
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "TEST.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("#define VISIBLE 1", rendered)
        self.assertIn("#define AFTER 2", rendered)
        self.assertNotIn("FIELD0", rendered)
        self.assertNotIn("FIELD1", rendered)
        self.assertNotIn("STRUCT\tTEST", rendered)

    def test_struct_definition_body_is_ignored_in_equ_header_render(self) -> None:
        equ_source = """*TEST.EQU
VISIBLE\t.set\t1
*STRUCT\tTEST
FIELD0\t.set\t0
FIELD1\t.set\t1
*ENDSTRUCT
AFTER\t.set\t2
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "TEST.EQU"
            src_path.write_text(equ_source)
            banner_comments, globls, sets = parse_equ_file(src_path)
            rendered = render_header(src_path, banner_comments, globls, sets, {})

        self.assertIn("// asm: VISIBLE\t.set\t1", rendered)
        self.assertIn("#define VISIBLE 1", rendered)
        self.assertIn("// asm: AFTER\t.set\t2", rendered)
        self.assertIn("#define AFTER 2", rendered)
        self.assertNotIn("FIELD0", rendered)
        self.assertNotIn("FIELD1", rendered)
        self.assertNotIn("STRUCT\tTEST", rendered)

    def test_first_function_claims_immediately_adjacent_comments_only(self) -> None:
        asm_source = """*COMMENT A\n*COMMENT B\nFIRST:\tRETS\n\n*COMMENT C\n\nSECOND:\tRETS\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "TEST.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("/* *COMMENT A\n*COMMENT B\n */\nvoid FIRST(void)", rendered)
        self.assertNotIn("/* *COMMENT C\n */\nvoid SECOND(void)", rendered)

    def test_same_entry_labels_render_as_alias_macros_and_one_function(self) -> None:
        asm_source = """*ALLOCATE PALETTES FOR A SECTION
SECTION_PALETTE_ALLOC:
alloc_section:
HARDalloc_section:\tLDI\t*AR2++,AR6
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "HUD.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)
            symbol_table = collect_module_symbol_table(src_path, {}, None)

        self.assertIn("#define SECTION_PALETTE_ALLOC HARDalloc_section", rendered)
        self.assertIn("#define alloc_section HARDalloc_section", rendered)
        self.assertIn("void HARDalloc_section(void)", rendered)
        self.assertNotIn("void SECTION_PALETTE_ALLOC(void)\n{", rendered)
        self.assertNotIn("void alloc_section(void)\n{", rendered)
        self.assertIn("/* *ALLOCATE PALETTES FOR A SECTION\n */\nvoid HARDalloc_section(void)", rendered)
        self.assertEqual(symbol_table["HARDalloc_section"].kind, "function")
        self.assertEqual(symbol_table["alloc_section"].kind, "define")
        self.assertEqual(symbol_table["alloc_section"].expr, "HARDalloc_section")
        self.assertEqual(symbol_table["SECTION_PALETTE_ALLOC"].kind, "define")
        self.assertEqual(symbol_table["SECTION_PALETTE_ALLOC"].expr, "HARDalloc_section")

    def test_bare_labels_do_not_start_functions_or_aliases(self) -> None:
        asm_source = """ENTRY0
ENTRY1:\tLDI\t1,R0
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "TEST.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)
            symbol_table = collect_module_symbol_table(src_path, {}, None)

        self.assertNotIn("#define ENTRY0 ENTRY1", rendered)
        self.assertNotIn("void ENTRY0(void)", rendered)
        self.assertIn("void ENTRY1(void)", rendered)
        self.assertNotIn("ENTRY0", symbol_table)
        self.assertEqual(symbol_table["ENTRY1"].kind, "function")

    def test_branch_target_colon_label_after_return_starts_new_function(self) -> None:
        asm_source = """FIRST:\tRETS
SECOND:\tRETS
THIRD:\tB\tSECOND
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "TEST.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void FIRST(void)", rendered)
        self.assertIn("void SECOND(void)", rendered)
        self.assertIn("void THIRD(void)", rendered)

    def test_colon_word_label_renders_as_data_not_function(self) -> None:
        asm_source = """TABLE:\t.word\tONE,TWO
ONE:\t.string\t\"ONE\",0
TWO:\t.string\t\"TWO\",0
FUNC:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "TEST.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn('const char *TABLE[] = {\n    ONE, TWO,\n};', rendered)
        self.assertNotIn("void TABLE(void)", rendered)
        self.assertIn("void FUNC(void)", rendered)

    def test_colon_entry_aliases_survive_interleaved_comments(self) -> None:
        asm_source = """SEND0:
;\tcommented body
SEND1:
;\tcommented body
SEND2:\tLDI\t1,R0
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "TEST.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)
            symbol_table = collect_module_symbol_table(src_path, {}, None)

        self.assertIn("#define SEND0 SEND2", rendered)
        self.assertIn("#define SEND1 SEND2", rendered)
        self.assertIn("void SEND2(void)", rendered)
        self.assertEqual(symbol_table["SEND0"].expr, "SEND2")
        self.assertEqual(symbol_table["SEND1"].expr, "SEND2")

    def test_bare_branch_labels_are_preserved_inside_function(self) -> None:
        asm_source = """FUNC:\tLDI\t0,R0
LOOP
\tADDI\t1,R0
\tBLT\tLOOP
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "TEST.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void FUNC(void)", rendered)
        self.assertIn("LOOP:", rendered)
        self.assertIn("// asm: \tADDI\t1,R0", rendered)

    def test_top_level_data_claims_immediately_adjacent_comments_only(self) -> None:
        asm_source = """*----------------------------------------------------------------------------\n*DYNAMIC fLEX OBJECTS\nNEW_GROUPI\t.word\tNEW_GROUP\n\t.bss\tNEW_GROUP,1\n\n*ORPHAN\n\nOTHER\t.word\t1\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "OBJ.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("/* *----------------------------------------------------------------------------\n*DYNAMIC fLEX OBJECTS\n */\n/* asm: NEW_GROUP\t.bss\tNEW_GROUP,1 */", rendered)
        self.assertNotIn("/* *ORPHAN\n */\n/* asm: OTHER\t.word\t1 */", rendered)

    def test_standalone_word_data_omits_comment_only_lines_from_asm_preamble(self) -> None:
        asm_source = """CRT_REG_SETUP_STR
\t.word\t399|CRT_SETUP_ICSYNC\t;CRT_SETUP
\t.word\t01ffh\t\t;CRT_HADDRINC
;before syncing
;\t.word\t400|CRT_SETUP_ICSYNC\t;CRT_SETUP
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "CUSA.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertNotIn("int CRT_REG_SETUP_STR[2] = {", rendered)
        self.assertIn("int CRT_REG_SETUP_STR[] = {", rendered)
        self.assertIn("399|CRT_SETUP_ICSYNC, // CRT_SETUP", rendered)
        self.assertIn("0x01ff, // CRT_HADDRINC", rendered)
        self.assertNotIn("/* asm: \t;before syncing */", rendered)
        self.assertNotIn("/* asm: \t;\t.word\t400|CRT_SETUP_ICSYNC\t;CRT_SETUP */", rendered)

    def test_parse_type_overrides_file(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            overrides_path = Path(tmpdir) / "type-overrides.txt"
            overrides_path.write_text("# comment\n-PB1\nBONUS8()\ntime=time_ROM\nvoid *SWTAB;\nunsigned short FLAGS[16];\n")

            parsed = parse_type_overrides_file(overrides_path)

        self.assertTrue(parsed["PB1"].omit)
        self.assertTrue(parsed["BONUS8"].force_function)
        self.assertEqual(parsed["time"].rename_to, "time_ROM")
        self.assertEqual(parsed["SWTAB"].c_type, "void *")
        self.assertIsNone(parsed["SWTAB"].array_expr)
        self.assertEqual(parsed["FLAGS"].c_type, "unsigned short")
        self.assertEqual(parsed["FLAGS"].array_expr, "16")

    def test_parse_render_overrides_file(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            overrides_path = Path(tmpdir) / "render-overrides.txt"
            overrides_path.write_text(
                "%% DEMOTHANKS_LIST\n"
                "tDEMO_THANKS DEMOTHANKS_LIST[] = {\n"
                "    { 140, DT1 },\n"
                "};\n"
                "%% END\n"
            )

            parsed = parse_render_overrides_file(overrides_path)

        self.assertEqual(
            parsed["DEMOTHANKS_LIST"],
            [
                "tDEMO_THANKS DEMOTHANKS_LIST[] = {",
                "    { 140, DT1 },",
                "};",
            ],
        )

    def test_omit_override_suppresses_variable_generation(self) -> None:
        asm_source = """PB1\t.usect\tpbsss,1
PBSS_PTR\t.word\tPB1
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "CUSA.ASM"
            src_path.write_text(asm_source)
            overrides_path = Path(tmpdir) / "type-overrides.txt"
            overrides_path.write_text("-PB1\n")
            overrides = parse_type_overrides_file(overrides_path)
            rendered = render_module(src_path, {}, {}, None, False, overrides)
            symbol_table = collect_module_symbol_table(src_path, {}, overrides)

        self.assertNotIn("PB1", symbol_table)
        self.assertNotIn("int PB1", rendered)
        self.assertIn("int PBSS_PTR = PB1;", rendered)

    def test_type_override_changes_generated_variable_type(self) -> None:
        asm_source = """SWTAB\t.word\tCOIN1,COIN2
COIN1:\tRETS
COIN2:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "CUSA.ASM"
            src_path.write_text(asm_source)
            overrides_path = Path(tmpdir) / "type-overrides.txt"
            overrides_path.write_text("void *SWTAB;\n")
            overrides = parse_type_overrides_file(overrides_path)
            rendered = render_module(src_path, {}, {}, None, False, overrides)
            symbol_table = collect_module_symbol_table(src_path, {}, overrides)

        self.assertIn("void *SWTAB[] = {\n    COIN1, COIN2,\n};", rendered)
        self.assertEqual(symbol_table["SWTAB"].c_type, "void *")
        self.assertEqual(symbol_table["SWTAB"].array_expr, "2")

    def test_rename_override_changes_generated_identifier(self) -> None:
        asm_source = """time\t.word\t123
TIMEPTR\t.word\ttime
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "HUD.ASM"
            src_path.write_text(asm_source)
            overrides_path = Path(tmpdir) / "type-overrides.txt"
            overrides_path.write_text("time=time_ROM\n")
            overrides = parse_type_overrides_file(overrides_path)
            rendered = render_module(src_path, {}, {}, None, False, overrides)
            symbol_table = collect_module_symbol_table(src_path, {}, overrides)

        self.assertIn("int time_ROM = 123;", rendered)
        self.assertIn("int *TIMEPTR = time_ROM;", rendered)
        self.assertNotIn("int time = 123;", rendered)
        self.assertEqual(symbol_table["time"].name, "time_ROM")

    def test_discovered_labels_header_applies_rename_override(self) -> None:
        overrides = {"time": TypeOverride(name="time", c_type="", rename_to="time_ROM")}
        rendered = render_discovered_labels_header([LabelEntry(name="time", addr=0x1234)], overrides)

        self.assertIn("#define time_ROM 0x00001234", rendered)
        self.assertNotIn("#define time 0x00001234", rendered)

    def test_render_override_replaces_word_table_output(self) -> None:
        asm_source = """DT1\t.string\t"THANK YOU FOR PLAYING",0
DT2\t.string\t"CRUISIN USA",0
DT3\t.string\t"SNEAK PREVIEW",0
DEMOTHANKS_LIST\t.word\t140,DT1
\t.word\t180,DT2
\t.word\t220,DT3
"""

        render_overrides = {
            "DEMOTHANKS_LIST": [
                "tDEMO_THANKS DEMOTHANKS_LIST[] = {",
                "    { 140, DT1 },",
                "    { 180, DT2 },",
                "    { 220, DT3 },",
                "};",
            ],
        }

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "ATTRACTA.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None, render_overrides=render_overrides)

        self.assertIn("tDEMO_THANKS DEMOTHANKS_LIST[] = {\n    { 140, DT1 },\n    { 180, DT2 },\n    { 220, DT3 },\n};", rendered)
        self.assertNotIn("int DEMOTHANKS_LIST[] = {", rendered)
        self.assertIn('const char *DT1 = "THANK YOU FOR PLAYING";', rendered)

    def test_function_override_emits_function_pointer_table_and_prototypes(self) -> None:
        asm_source = """BONUS_TABLE\t.word\tBONUS8

BONUS8\tLDI\t8,R1
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            overrides_path = Path(tmpdir) / "type-overrides.txt"
            overrides_path.write_text("BONUS8()\n")
            overrides = parse_type_overrides_file(overrides_path)
            rendered = render_module(src_path, {}, {}, None, False, overrides)

        self.assertIn("void BONUS8(void);", rendered)
        self.assertIn("void (*BONUS_TABLE)(void) = BONUS8;", rendered)
        self.assertIn("void BONUS8(void)", rendered)

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

    def test_render_discovered_labels_header_uses_hex_addresses(self) -> None:
        rendered = render_discovered_labels_header([
            LabelEntry(name="missle", addr=0x00C28DF4),
            LabelEntry(name="misslem", addr=0x00C29323),
        ])

        self.assertIn("#define missle 0x00C28DF4", rendered)
        self.assertIn("#define misslem 0x00C29323", rendered)

    def test_render_header_emits_function_alias_macro_for_globl(self) -> None:
        asm_source = """SECTION_PALETTE_ALLOC:\nalloc_section:\nHARDalloc_section:\tRETS\n"""
        equ_source = """.globl SECTION_PALETTE_ALLOC\n.globl alloc_section\n.globl HARDalloc_section\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            asm_path = Path(tmpdir) / "HUD.ASM"
            equ_path = Path(tmpdir) / "GLOBALS.EQU"
            asm_path.write_text(asm_source)
            equ_path.write_text(equ_source)
            banner_comments, globls, sets = parse_equ_file(equ_path)
            symbol_table = collect_module_symbol_table(asm_path, {}, None)
            rendered = render_header(equ_path, banner_comments, globls, sets, symbol_table)

        self.assertIn("void HARDalloc_section(void);", rendered)
        self.assertIn("#define alloc_section HARDalloc_section", rendered)
        self.assertIn("#define SECTION_PALETTE_ALLOC HARDalloc_section", rendered)

    def test_collect_referenced_define_symbols(self) -> None:
        asm_lines = [
            "\tLDI\tbottom_gtmp_p,R0",
            "\tADDI\tcam_left_stop,R1",
            "\tCALL\tSOMEFUNC",
        ]

        refs = collect_referenced_define_symbols(asm_lines, {"bottom_gtmp_p", "cam_left_stop"})

        self.assertEqual(refs, {"bottom_gtmp_p", "cam_left_stop"})

    def test_collect_existing_macro_names(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            include_dir = Path(tmpdir)
            (include_dir / "one.h").write_text("#define ATOD_R 0x0993000\n#define NULL ((void*)0)\n")
            (include_dir / "two.h").write_text(" # define SCREEN0 0x0900000\n")
            (include_dir / "discovered_labels.h").write_text("#define missle 0x00C28DF4\n")

            macros = collect_existing_macro_names(include_dir)

        self.assertIn("ATOD_R", macros)
        self.assertIn("NULL", macros)
        self.assertIn("SCREEN0", macros)
        self.assertNotIn("missle", macros)

    def test_collect_source_label_names_includes_bare_source_labels(self) -> None:
        asm_lines = [
            "ADJUSTMENT_MENU",
            '\tMENUENTRY\t"STANDARD PRICING",RUN_STANDARD_PRICING',
            "RUN_ADJUSTMENT_MENU:",
            "\tLDL\tADJUSTMENT_MENU,AR5",
            "\t.bss\tWAS_HEAD2HEAD_ON,1",
        ]

        labels = collect_source_label_names(asm_lines)

        self.assertIn("ADJUSTMENT_MENU", labels)
        self.assertIn("RUN_ADJUSTMENT_MENU", labels)
        self.assertIn("WAS_HEAD2HEAD_ON", labels)

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

    def test_equ_file_renders_comment_header_without_set_defines(self) -> None:
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
        self.assertIn("// asm: MAX_DRONES\t.set\t11\t;MAXIMUM DRONES IN UNIVERSE", rendered)
        self.assertNotIn("#define DELTA_SAFETYWIDTH 850", rendered)
        self.assertNotIn("#define MAX_DRONES 11 //MAXIMUM DRONES IN UNIVERSE", rendered)
        self.assertNotIn("void delta(void)", rendered)

    def test_render_function_omits_fallthrough_warning_after_return(self) -> None:
        asm_source = """SERIOUSLY_NORMALIZE:\tABSF\tR0,R1
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "ATTRDRNE.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void SERIOUSLY_NORMALIZE(void)", rendered)
        self.assertNotIn("// WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION", rendered)

    def test_render_function_adds_fallthrough_warning_after_plain_instruction(self) -> None:
        asm_source = """SET_SMOOTH_VIEW:\tLDI\t0,AR5
\tSTF\tR3,*+AR7(CAMERA_XYZR+3)

SMOOTH_VIEW:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "ATTRDRNE.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void SET_SMOOTH_VIEW(void)", rendered)
        self.assertIn("    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION", rendered)

    def test_render_function_adds_fallthrough_warning_after_unconditional_branch(self) -> None:
        asm_source = """FUNC1:\tBR\tNEXTFN

NEXTFN:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "ATTRDRNE.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(
                src_path,
                {},
                {},
                None,
                False,
                {"NEXTFN": TypeOverride(name="NEXTFN", c_type="", force_function=True)},
            )

        func1_body = self.extract_function_body(rendered, "FUNC1")
        self.assertIn("// WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION", func1_body)

    def test_render_function_adds_fallthrough_warning_after_conditional_branch(self) -> None:
        asm_source = """FUNC1:\tBEQ\tNEXTFN

NEXTFN:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "ATTRDRNE.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(
                src_path,
                {},
                {},
                None,
                False,
                {"NEXTFN": TypeOverride(name="NEXTFN", c_type="", force_function=True)},
            )

        func1_body = self.extract_function_body(rendered, "FUNC1")
        self.assertIn("// WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION", func1_body)

    def test_render_function_warns_on_split_fallthrough_but_not_terminal_successor(self) -> None:
        asm_source = """SET_SMOOTH_VIEW:\tLDI\t0,AR5
\tBEQ\tSMOOTH_VIEW
\tSTF\tR3,*+AR7(CAMERA_XYZR+3)
SMOOTH_VIEW:\tCALL\tZOOM_CAMERA
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "ATTRDRNE.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None, False, {"SMOOTH_VIEW": TypeOverride(name="SMOOTH_VIEW", c_type="", force_function=True)})

        first_body = self.extract_function_body(rendered, "SET_SMOOTH_VIEW")
        second_body = self.extract_function_body(rendered, "SMOOTH_VIEW")
        self.assertIn("// WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION", first_body)
        self.assertNotIn("// WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION", second_body)


if __name__ == "__main__":
    unittest.main()
