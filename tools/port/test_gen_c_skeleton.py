#!/usr/bin/env python3

import re
import tempfile
import unittest
from pathlib import Path

from gen_c_skeleton import (
    DefineEntry,
    LabelEntry,
    StorageVariable,
    SymbolInfo,
    TypeOverride,
    collect_existing_macro_names,
    collect_source_label_names,
    collect_defined_data_symbols,
    collect_module_symbol_table,
    collect_referenced_define_symbols,
    collect_referenced_data_symbols,
    parse_discovered_defines_file,
    parse_equ_file,
    parse_instruction_addresses_file,
    parse_render_overrides_file,
    parse_string_blob_operand,
    parse_type_overrides_file,
    classify_instruction_text,
    file_has_noedit_guard,
    instruction_text_for_top_level_line,
    resolve_generated_output_path,
    render_equ_header,
    merge_storage_into_header,
    render_macro_data_placeholder,
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

    def test_file_has_noedit_guard_only_when_first_line_matches(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            guarded = Path(tmpdir) / "guarded.c"
            guarded.write_text("// NOEDIT\nint x;\n")
            unguarded = Path(tmpdir) / "unguarded.c"
            unguarded.write_text("int x;\n// NOEDIT\n")

            self.assertTrue(file_has_noedit_guard(guarded))
            self.assertFalse(file_has_noedit_guard(unguarded))

    def test_resolve_generated_output_path_redirects_guarded_targets(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            target = tmp / "src" / "game" / "cointab.c"
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text("// NOEDIT\n")
            generated_dir = tmp / "src" / "generated"

            resolved = resolve_generated_output_path(target, generated_dir)

            self.assertEqual(resolved, generated_dir / "cointab.c")

    def test_resolve_generated_output_path_keeps_unguarded_targets(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            target = tmp / "src" / "game" / "coin.h"
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text("/* generated */\n")
            generated_dir = tmp / "src" / "generated"

            resolved = resolve_generated_output_path(target, generated_dir)

            self.assertEqual(resolved, target)

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

        var1_idx = rendered.rindex("int VAR1;")
        func1_idx = rendered.index("void FUNC1(void)\n{")
        var2_idx = rendered.rindex("int VAR2;")
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
        self.assertIn('const char LINKDISABLED[] = "LINK DISABLED BY U97  DIP6 OFF";', rendered)
        self.assertIn('const char IAMMASTER[] = "LINK MASTER MACHINE";', rendered)
        self.assertIn('void MSG3(void)', rendered)
        self.assertNotIn('LINKDISABLED:\n    // asm: SPTR\t"LINK DISABLED BY U97  DIP6 OFF"', rendered)
        self.assertNotIn('\nchar *LINKDISABLED = "LINK DISABLED BY U97  DIP6 OFF";', rendered)
        self.assertEqual(rendered.count("// asm: LDI\t11,RC"), 2)

    def test_bare_label_followed_by_sptr_rows_renders_string_pointer_array(self) -> None:
        asm_source = """STATION_TEXT
\tSPTR\t"SURFARI MONSTER"\t;MONSTER_SURF\t;0
\tSPTR\t"REDLINE SHUFFLE"\t;SHUFFLE_DRIV\t;1
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "SND.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)
            symbol_table = collect_module_symbol_table(src_path, {})

        self.assertEqual(symbol_table["STATION_TEXT"].c_type, "const char *")
        self.assertEqual(symbol_table["STATION_TEXT"].array_expr, "2")
        self.assertIn("const char *STATION_TEXT[] = {", rendered)
        self.assertIn('    "SURFARI MONSTER", // MONSTER_SURF\t;0', rendered)
        self.assertIn('    "REDLINE SHUFFLE", // SHUFFLE_DRIV\t;1', rendered)

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

    def test_instruction_addresses_survive_stripped_macro_blocks_and_alias_labels(self) -> None:
        asm_source = """MAKE_NOP\t.MACRO
\tNOP
\t.ENDM

ENTRY:
ALIAS:
\tLDI\t@_MODE,R0
\tOR\tMWATER,R0
\tSTI\tR0,@_MODE
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(
                src_path,
                {},
                {},
                None,
                False,
                None,
                False,
                {7: 0x0000396B, 8: 0x0000396C, 9: 0x0000396D},
            )

        self.assertIn("void ALIAS(void)\n{", rendered)
        self.assertIn("    // asm 0000396B: \tLDI\t@_MODE,R0", rendered)
        self.assertIn("    // asm 0000396C: \tOR\tMWATER,R0", rendered)
        self.assertIn("    // asm 0000396D: \tSTI\tR0,@_MODE", rendered)

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

    def test_standalone_label_word_forward_declaration_matches_array_shape(self) -> None:
        asm_source = """CONGRAT_SPEECH:\t.word\tGL_WOOLAUGH,GL_YEAH,GL_YES,GL_YOUDIDIT
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertNotIn("static int CONGRAT_SPEECH[4];", rendered)
        self.assertIn("static int CONGRAT_SPEECH[] = {\n    GL_WOOLAUGH, GL_YEAH, GL_YES, GL_YOUDIDIT,\n};", rendered)
        self.assertNotIn("extern int CONGRAT_SPEECH;", rendered)

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

    def test_float_table_preserves_disabled_and_row_comments(self) -> None:
        asm_source = """TABLEPTR\t.word\tTABLE

*STDARD .float\t0.82,1.00,0.0028,0.010
*NEWSTD\t.float\t0.82,0.90,0.0028,0.0060

TABLE:
*#0 MUSCLE CAR
\t.float\t0.91,0.60,0.0028,0.010\t\t;ALL AROUND
*#1 XXX
\t.float\t0.98,0.50,0.0032,0.0042\t\t;ACCEL
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "PLYR.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("/*\n*STDARD .float\t0.82,1.00,0.0028,0.010\n*NEWSTD\t.float\t0.82,0.90,0.0028,0.0060\n*/", rendered)
        self.assertIn("float TABLE[] = {\n    // #0 MUSCLE CAR\n    0.91f, 0.60f, 0.0028f, 0.010f, // ALL AROUND\n    // #1 XXX\n    0.98f, 0.50f, 0.0032f, 0.0042f, // ACCEL\n};", rendered)

    def test_bare_label_single_row_float_table_stays_float(self) -> None:
        asm_source = """GEARACTABI\t.word\tGEARACTAB
GEARACTAB
\t.float\t0.0,1.7,1.5,1.4,1.2
NEXT\t.word\t1
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "PLYR.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)
            symbol_table = collect_module_symbol_table(src_path, {})

        self.assertEqual(symbol_table["GEARACTAB"].c_type, "float")
        self.assertIn("float GEARACTAB[] = {\n    0.0f, 1.7f, 1.5f, 1.4f, 1.2f,\n};", rendered)

    def test_string_label_table_keeps_unreferenced_children(self) -> None:
        asm_source = """LEG_NAMES\t.word\tLEG1,LEG2
LEG1\t.string\t"GOLDEN GATE PARK",0
LEG2\t.string\t"SAN FRANCISCO",0
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("const char *LEG_NAMES[] = {\n    LEG1, LEG2,\n};", rendered)
        self.assertIn('const char LEG1[] = "GOLDEN GATE PARK";', rendered)
        self.assertIn('const char LEG2[] = "SAN FRANCISCO";', rendered)

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
        self.assertIn('const char LEG1[] = "GOLDEN GATE PARK";', rendered)
        self.assertIn('const char LEG2[] = "SAN FRANCISCO";', rendered)

    def test_parse_string_blob_operand_decodes_mixed_string_and_byte_suffix(self) -> None:
        self.assertEqual(parse_string_blob_operand('"ST XYZ;MO",13,0'), r'"ST XYZ;MO\n"')

    def test_mixed_string_blob_renders_as_char_array(self) -> None:
        asm_source = 'MOTOROFF12\t.string\t"ST XYZ;MO",13,0\n'

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "MOTION.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn('const char MOTOROFF12[] = "ST XYZ;MO\\n";', rendered)
        self.assertNotIn("int MOTOROFF12[] = {", rendered)

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

    def test_classify_instruction_text_uses_remainder_after_flush_left_label(self) -> None:
        self.assertEqual(classify_instruction_text("BABAB\tBZ\tNEXT"), (True, "BZ\tNEXT"))
        self.assertEqual(classify_instruction_text("TABLE\t.word\t1,2"), (False, ""))
        self.assertEqual(classify_instruction_text("LDI\t*AR1++(6),R1"), (True, "LDI\t*AR1++(6),R1"))

    def test_instruction_text_for_top_level_line_treats_labeled_word_as_data(self) -> None:
        self.assertIsNone(instruction_text_for_top_level_line("TABLE\t.word\t1,2"))
        self.assertEqual(instruction_text_for_top_level_line("BABAB\tBZ\tNEXT"), "BZ\tNEXT")

    def test_comment_struct_without_endstruct_does_not_truncate_file(self) -> None:
        asm_source = """TABLE0\t.word\t1
*STRUCT VEHTAB
*       .word   model
TABLE1\t.word\t2
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "WAVE.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("static int TABLE0 = 1;", rendered)
        self.assertIn("static int TABLE1 = 2;", rendered)

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
        self.assertIn("/* asm: \t.word\tENTRY1 */", rendered)
        self.assertIn('/* asm: \t.string\t"A",0 */', rendered)
        self.assertIn("static int AUDIT_LIST[] = {\n    ENTRY1,\n    0x00000041,\n};", rendered)
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

        self.assertNotIn('#include "port.h"', rendered)
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

        self.assertNotIn('#include "port.h"', rendered)
        self.assertNotIn('#include "discovered_labels.h"', rendered)
        self.assertNotIn("#define missle", rendered)

    def test_render_module_marks_non_exported_functions_static(self) -> None:
        asm_source = """EXPORTED:\tRETS
INTERNAL:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            asm_dir = tmp / "asm"
            include_dir = tmp / "src" / "game"
            asm_dir.mkdir(parents=True, exist_ok=True)
            include_dir.mkdir(parents=True, exist_ok=True)
            src_path = asm_dir / "LOCALS.ASM"
            src_path.write_text(asm_source)
            (include_dir / "locals.h").write_text("void EXPORTED(void);\n")

            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void EXPORTED(void);", rendered)
        self.assertIn("void EXPORTED(void)\n{", rendered)
        self.assertIn("static void INTERNAL(void);", rendered)
        self.assertIn("static void INTERNAL(void)\n{", rendered)

    def test_module_globl_function_stays_external_without_header_proto(self) -> None:
        asm_source = """.globl\tDECODE_FLY_KILL
DECODE_FLY_KILL:\tRETS
LOCAL_ONLY:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "COLLA.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("void DECODE_FLY_KILL(void);", rendered)
        self.assertIn("void DECODE_FLY_KILL(void)\n{", rendered)
        self.assertIn("static void LOCAL_ONLY(void);", rendered)
        self.assertIn("static void LOCAL_ONLY(void)\n{", rendered)

    def test_non_exported_top_level_data_becomes_static(self) -> None:
        asm_source = """FORMULA\t.float\t1.25
USEIT:\tMPYF\t@FORMULA,R0
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "MATH.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertNotIn("static float FORMULA;", rendered)
        self.assertIn("static float FORMULA = 1.25f;", rendered)

    def test_non_exported_top_level_data_without_early_use_skips_forward_declaration(self) -> None:
        asm_source = """FORMULA\t.float\t1.25
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "MATH.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertNotIn("static float FORMULA;", rendered)
        self.assertIn("static float FORMULA = 1.25f;", rendered)

    def test_word_alias_to_immediately_following_variable_skips_forward_declaration(self) -> None:
        asm_source = """BUFFERSI\t.word\tBIGBUFFER
\t.bss\tBIGBUFFER,8
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("#define BUFFERSI BIGBUFFER", rendered)
        self.assertNotIn("static int BIGBUFFER;", rendered)
        self.assertIn("static int BIGBUFFER[8];", rendered)

    def test_word_alias_to_later_variable_skips_forward_declaration_without_intervening_use(self) -> None:
        asm_source = """BUFFERSI\t.word\tBIGBUFFER
* comment
\t.if\tDEBUG
\t.endif
\t.bss\tBIGBUFFER,8
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("#define BUFFERSI BIGBUFFER", rendered)
        self.assertNotIn("static int BIGBUFFER;", rendered)
        self.assertIn("static int BIGBUFFER[8];", rendered)

    def test_header_exported_top_level_data_stays_external(self) -> None:
        asm_source = """\tpbss\tATTRWAVE,1
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            asm_dir = tmp / "asm"
            include_dir = tmp / "src" / "game"
            asm_dir.mkdir(parents=True, exist_ok=True)
            include_dir.mkdir(parents=True, exist_ok=True)
            src_path = asm_dir / "ATTRDRNE.ASM"
            src_path.write_text(asm_source)
            (include_dir / "attrdrne.h").write_text("extern int ATTRWAVE;\n")

            rendered = render_module(src_path, {}, {}, None)

        self.assertNotIn("extern int ATTRWAVE;", rendered)
        self.assertNotIn("static int ATTRWAVE;", rendered)
        self.assertIn("int ATTRWAVE;", rendered)

    def test_non_exported_macrodata_placeholder_becomes_static(self) -> None:
        rendered = render_macro_data_placeholder(
            "AUDIT_LIST",
            ["AUDIT_LIST", '\tAUDENT\tAUD_COIN1,"LEFT COIN"'],
        )

        self.assertIn("static int AUDIT_LIST;", rendered)

    def test_rgb_macro_table_renders_as_integer_array(self) -> None:
        asm_source = """RGB\t.MACRO\tR,G,B
\t.word\t(((:R:>>3)&01Fh)<<10)|(((:G:>>3)&01Fh)<<5)|(((:B:>>3)&01Fh))
\t.ENDM

FIXEDPAL:
\t.word\t2
\tRGB\t0,0,0
\tRGB\t255,255,255
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "WAVE.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("static int FIXEDPAL[] = {", rendered)
        self.assertIn("    2,", rendered)
        self.assertIn("    (((0>>3)&0x01F)<<10)|(((0>>3)&0x01F)<<5)|(((0>>3)&0x01F)),", rendered)
        self.assertIn("    (((255>>3)&0x01F)<<10)|(((255>>3)&0x01F)<<5)|(((255>>3)&0x01F)),", rendered)

    def test_romdata_macro_terminates_numeric_table_without_placeholder(self) -> None:
        asm_source = """romdata\t.MACRO
\t.sect\t"THEDATA"
\t.ENDM

TABLEI
\t.word\tTABLE
\tromdata
TABLE:
\t.word\t1,2
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "SETUPS.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("#define TABLEI TABLE", rendered)
        self.assertIn("static int TABLE[] = {\n    1, 2,\n};", rendered)
        self.assertNotIn("static int TABLEI;", rendered)

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
        self.assertNotIn('const char l_[] = ":ATEXT:";', rendered)
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
            rendered = render_equ_header(src_path, banner_comments, globls, sets, {})

        self.assertIn('#include "port.h"', rendered)
        self.assertIn("// asm: VISIBLE\t.set\t1", rendered)
        self.assertIn("#define VISIBLE 1", rendered)
        self.assertIn("// asm: AFTER\t.set\t2", rendered)
        self.assertIn("#define AFTER 2", rendered)
        self.assertNotIn("FIELD0", rendered)
        self.assertNotIn("FIELD1", rendered)
        self.assertNotIn("STRUCT\tTEST", rendered)

    def test_set_define_preserves_hex_literal_format(self) -> None:
        asm_source = """FRAME\t.set\t85h
SCROLLB\t.set\t86h
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "HSTDP.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("#define FRAME 0x85", rendered)
        self.assertIn("#define SCROLLB 0x86", rendered)
        self.assertNotIn("#define FRAME 133", rendered)

    def test_first_function_claims_immediately_adjacent_comments_only(self) -> None:
        asm_source = """*COMMENT A\n*COMMENT B\nFIRST:\tRETS\n\n*COMMENT C\n\nSECOND:\tRETS\n"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "TEST.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("/*\n*COMMENT A\n*COMMENT B\n*/\nstatic void FIRST(void)", rendered)
        self.assertNotIn("/*\n*COMMENT C\n*/\nstatic void SECOND(void)", rendered)

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
        self.assertIn("static void HARDalloc_section(void)", rendered)
        self.assertNotIn("void SECTION_PALETTE_ALLOC(void)\n{", rendered)
        self.assertNotIn("void alloc_section(void)\n{", rendered)
        self.assertIn("// *ALLOCATE PALETTES FOR A SECTION\nstatic void HARDalloc_section(void)", rendered)
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

        self.assertIn("/*\n*----------------------------------------------------------------------------\n*DYNAMIC fLEX OBJECTS\n*/\n/* asm: NEW_GROUP\t.bss\tNEW_GROUP,1 */", rendered)
        self.assertNotIn("/*\n*ORPHAN\n*/\n/* asm: OTHER\t.word\t1 */", rendered)

    def test_single_preserved_top_level_comment_renders_as_cpp_comment(self) -> None:
        asm_source = """*NO MORE UNLESS NOT JSRPing

THING\t.word\t1
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "TEST.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("// *NO MORE UNLESS NOT JSRPing", rendered)
        self.assertNotIn("/*\n*NO MORE UNLESS NOT JSRPing\n*/", rendered)

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
            overrides_path.write_text(
                "# comment\n-PB1\nBONUS8()\ntime=time_ROM\nvoid *SWTAB;\nunsigned short FLAGS[16];\nintptr_t GGPARK_LIST[];\n"
            )

            parsed = parse_type_overrides_file(overrides_path)

        self.assertTrue(parsed["PB1"].omit)
        self.assertTrue(parsed["BONUS8"].force_function)
        self.assertEqual(parsed["time"].rename_to, "time_ROM")
        self.assertEqual(parsed["SWTAB"].c_type, "void *")
        self.assertIsNone(parsed["SWTAB"].array_expr)
        self.assertEqual(parsed["FLAGS"].c_type, "unsigned short")
        self.assertEqual(parsed["FLAGS"].array_expr, "16")
        self.assertEqual(parsed["GGPARK_LIST"].c_type, "intptr_t")
        self.assertEqual(parsed["GGPARK_LIST"].array_expr, "")

    def test_variable_declaration_preserves_unsized_array_override(self) -> None:
        self.assertEqual(variable_declaration("TABLE", "int", ""), "int TABLE[];")
        self.assertEqual(variable_declaration("TABLE", "int", "", is_extern=True), "extern int TABLE[];")
        self.assertEqual(variable_declaration("TABLE", "void (*)(void)", ""), "void (*TABLE[])(void);")

    def test_storage_type_override_without_brackets_suppresses_inferred_array(self) -> None:
        asm_source = """\thibss\tDICT,TABLE_SIZE*DICT_SIZ
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "COMP.ASM"
            src_path.write_text(asm_source)
            overrides_path = Path(tmpdir) / "type-overrides.txt"
            overrides_path.write_text("tDICT DICT;\n")
            overrides = parse_type_overrides_file(overrides_path)
            symbol_table = collect_module_symbol_table(src_path, {}, overrides)
            rendered = render_storage_header(src_path, [StorageVariable("DICT", 0, "COMP", "\thibss\tDICT,TABLE_SIZE*DICT_SIZ", "TABLE_SIZE*DICT_SIZ")], overrides)

        self.assertEqual(symbol_table["DICT"].c_type, "tDICT")
        self.assertIsNone(symbol_table["DICT"].array_expr)
        self.assertIn("extern tDICT DICT;", rendered)
        self.assertNotIn("extern tDICT DICT[];", rendered)

    def test_uintptr_word_table_casts_function_entries(self) -> None:
        asm_source = """INIT_STARTING:\tRETS
ROAD_VIEW:\tRETS
GGPARK_LIST\t.word\tINIT_STARTING,70,ROAD_VIEW
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "ATTRDRNE.ASM"
            src_path.write_text(asm_source)
            type_overrides = parse_type_overrides_file(Path("/dev/null"))
            type_overrides["GGPARK_LIST"] = TypeOverride(name="GGPARK_LIST", c_type="uintptr_t", array_expr="")
            rendered = render_module(src_path, {}, {}, None, type_overrides=type_overrides)

        self.assertIn("uintptr_t GGPARK_LIST[] = {", rendered)
        self.assertIn("    (uintptr_t)INIT_STARTING, 70, (uintptr_t)ROAD_VIEW,", rendered)

    def test_uintptr_word_table_casts_cross_module_function_entries(self) -> None:
        asm_source = """ROUTINE_TAB\t.word\t470h,RRSTART_ENGINE
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BACKGRND.ASM"
            src_path.write_text(asm_source)
            type_overrides = parse_type_overrides_file(Path("/dev/null"))
            type_overrides["ROUTINE_TAB"] = TypeOverride(name="ROUTINE_TAB", c_type="uintptr_t", array_expr="")
            rendered = render_module(
                src_path,
                {},
                {},
                None,
                type_overrides=type_overrides,
                global_symbol_table={
                    "RRSTART_ENGINE": SymbolInfo(name="RRSTART_ENGINE", kind="function", module="RROAD"),
                },
            )

        self.assertIn("uintptr_t ROUTINE_TAB[] = {", rendered)
        self.assertIn("    0x470, (uintptr_t)RRSTART_ENGINE,", rendered)

    def test_word_table_of_predeclared_defines_stays_int_array(self) -> None:
        asm_source = """STATION_LIST\t.word\tMUNSTER_SURF,SHUFFLE_DRIV
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp_path = Path(tmpdir)
            asm_dir = tmp_path / "asm"
            include_dir = tmp_path / "src" / "game"
            asm_dir.mkdir()
            include_dir.mkdir(parents=True)
            src_path = asm_dir / "SND.ASM"
            src_path.write_text(asm_source)
            (include_dir / "port.h").write_text("#define PORT_H\n#define MUNSTER_SURF 222\n#define SHUFFLE_DRIV 223\n")
            symbol_table = collect_module_symbol_table(
                src_path,
                {},
                predefined_define_names={"MUNSTER_SURF", "SHUFFLE_DRIV"},
            )
            rendered = render_module(
                src_path,
                {},
                {},
                None,
            )

        self.assertEqual(symbol_table["STATION_LIST"].c_type, "int")
        self.assertIn("int STATION_LIST[] = {", rendered)
        self.assertNotIn("uintptr_t STATION_LIST[] = {", rendered)

    def test_flush_left_branch_target_label_starting_with_b_renders_as_c_label(self) -> None:
        asm_source = """WAIT_FOR_CHALLENGER:\tLDI\t@HEAD2HEAD_ON,R0
\tBZ\tBABAB
\tLDI\t@OM_VEHICLE,R0
BABAB
\tBNZ\tHHFBF
HHFBF:\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "INTRO.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("    // asm: \tBZ\tBABAB", rendered)
        self.assertIn("BABAB:", rendered)
        self.assertNotIn("// asm: BABAB", rendered)

    def test_inline_label_keeps_trailing_comment_on_instruction(self) -> None:
        asm_source = """FUNC:\tRPTB\tBLOOPER
BLOOPER\tLDI\t*--AR4,R0\t;and this becomes a pre-decrement
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "COMP.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("BLOOPER:", rendered)
        self.assertIn("    // asm: LDI\t*--AR4,R0\t;and this becomes a pre-decrement", rendered)

    def test_mixed_inline_and_bare_labeled_numeric_records_render_as_separate_arrays(self) -> None:
        asm_source = """COIN_TABLE
USA1\t.string\t1,1,4,4
\t.string\t3,0,0,1
\t.word\tMSG_13
\t.string\t1,1,4,4

USA2\t.string\t1,1,4,4
\t.string\t2,0,0,1
\t.word\tMSG_12
\t.string\t1,1,4,4

USA3
\t.string\t1,1,4,4
\t.string\t4,0,0,1
\t.word\tMSG_14
\t.string\t1,1,4,4

USA4
\t.string\t1,1,4,4
\t.string\t5,0,0,1
\t.word\tMSG_15
\t.string\t1,1,4,4
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "COINTAB.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)
            symbol_table = collect_module_symbol_table(src_path, {})

        for name in ("USA1", "USA2", "USA3", "USA4"):
            self.assertEqual(symbol_table[name].kind, "variable")
            self.assertIn(f"int {name}[] = {{", rendered)
        self.assertNotIn("#define USA2", rendered)
        self.assertNotIn("#define USA3", rendered)
        self.assertNotIn("#define USA4", rendered)

    def test_separator_comment_between_numeric_records_stays_with_following_record(self) -> None:
        asm_source = """COIN_TABLE
;1/3X25
USA1\t.string\t1,1,4,4
\t.string\t3,0,0,1
\t.word\tMSG_13
\t.string\t1,1,4,4

;1/2X25
USA2\t.string\t1,1,4,4
\t.string\t2,0,0,1
\t.word\tMSG_12
\t.string\t1,1,4,4
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "COINTAB.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(src_path, {}, {}, None)

        self.assertIn("// ;1/2X25\n/* asm: USA2\t.string\t1,1,4,4 */", rendered)

    def test_uintptr_word_table_casts_local_string_entries_with_address_of(self) -> None:
        asm_source = """STRING_TAB\t.word\tLEG1
LEG1\t.string\t"GOLDEN GATE PARK",0
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            type_overrides = parse_type_overrides_file(Path("/dev/null"))
            type_overrides["STRING_TAB"] = TypeOverride(name="STRING_TAB", c_type="uintptr_t", array_expr="")
            rendered = render_module(src_path, {}, {}, None, type_overrides=type_overrides)

        self.assertIn("uintptr_t STRING_TAB = (uintptr_t)&LEG1;", rendered)
        self.assertIn('const char LEG1[] = "GOLDEN GATE PARK";', rendered)

    def test_uintptr_word_table_casts_cross_module_string_entries_with_address_of(self) -> None:
        asm_source = """STRING_TAB\t.word\tLEG1
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            type_overrides = parse_type_overrides_file(Path("/dev/null"))
            type_overrides["STRING_TAB"] = TypeOverride(name="STRING_TAB", c_type="uintptr_t", array_expr="")
            rendered = render_module(
                src_path,
                {},
                {},
                None,
                type_overrides=type_overrides,
                global_symbol_table={
                    "LEG1": SymbolInfo(name="LEG1", kind="variable", module="TEXT", c_type="const char", array_expr=""),
                },
            )

        self.assertIn("uintptr_t STRING_TAB = (uintptr_t)&LEG1;", rendered)

    def test_uintptr_word_table_casts_model_alias_to_function(self) -> None:
        asm_source = """BONUS_POSTLAUNCH\t.word\tBONUS_GGATE,BONUS_BEVHILLS
BONUS_GGATE:
BONUS_BEVHILLS:
\tRETS
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "BONUS.ASM"
            src_path.write_text(asm_source)
            type_overrides = parse_type_overrides_file(Path("/dev/null"))
            type_overrides["BONUS_POSTLAUNCH"] = TypeOverride(name="BONUS_POSTLAUNCH", c_type="uintptr_t", array_expr="")
            rendered = render_module(src_path, {}, {}, None, type_overrides=type_overrides)
            symbol_table = collect_module_symbol_table(src_path, {}, type_overrides)

        self.assertEqual(symbol_table["BONUS_GGATE"].kind, "define")
        self.assertEqual(symbol_table["BONUS_GGATE"].expr, "BONUS_BEVHILLS")
        self.assertEqual(symbol_table["BONUS_BEVHILLS"].kind, "function")
        self.assertIn("uintptr_t BONUS_POSTLAUNCH[] = {", rendered)
        self.assertIn("    (uintptr_t)BONUS_GGATE, (uintptr_t)BONUS_BEVHILLS,", rendered)

    def test_render_module_emits_local_variable_forward_declarations(self) -> None:
        asm_source = """VIEWLIST\t.word\tGGPARK_LIST,GGPARK_LIST
GGPARK_LIST\t.word\t1,2,3
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "ATTRDRNE.ASM"
            src_path.write_text(asm_source)
            type_overrides = parse_type_overrides_file(Path("/dev/null"))
            type_overrides["VIEWLIST"] = TypeOverride(name="VIEWLIST", c_type="uintptr_t *", array_expr="")
            type_overrides["GGPARK_LIST"] = TypeOverride(name="GGPARK_LIST", c_type="uintptr_t", array_expr="")
            rendered = render_module(src_path, {}, {}, None, type_overrides=type_overrides)

        self.assertNotIn("static uintptr_t *VIEWLIST[];", rendered)
        self.assertIn("static uintptr_t GGPARK_LIST[];", rendered)
        self.assertIn("static uintptr_t *VIEWLIST[] = {", rendered)
        self.assertIn("static uintptr_t GGPARK_LIST[] = {", rendered)

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
        self.assertIn('const char DT1[] = "THANK YOU FOR PLAYING";', rendered)

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

        self.assertIn("#define missle_ROM 0x00C28DF4", rendered)
        self.assertIn("#define misslem_ROM 0x00C29323", rendered)

    def test_render_module_uses_discovered_label_rom_names_in_references(self) -> None:
        asm_source = """MISSLEPAL\t.word\tmissle
"""

        with tempfile.TemporaryDirectory() as tmpdir:
            src_path = Path(tmpdir) / "MISSLE.ASM"
            src_path.write_text(asm_source)
            rendered = render_module(
                src_path,
                {"missle": 0x00C28DF4},
                {},
                None,
                discovered_labels_needed=True,
                discovered_label_names={"missle"},
            )

        self.assertIn("int MISSLEPAL = missle_ROM;", rendered)
        self.assertNotIn("int MISSLEPAL = missle;", rendered)


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
            rendered = render_equ_header(equ_path, banner_comments, globls, sets, symbol_table)

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

        self.assertIn('#include "port.h"', rendered)
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
