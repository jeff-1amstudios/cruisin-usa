#!/usr/bin/env python3

import tempfile
import unittest
from pathlib import Path
import sys

TOOLS_DIR = Path(__file__).resolve().parent
if str(TOOLS_DIR) not in sys.path:
    sys.path.insert(0, str(TOOLS_DIR))

from check_asm_source_sync import (
    compare_pair,
    is_asm_code_line,
    read_asm_code_lines,
    source_module_for_c,
)


class CheckAsmSourceSyncTests(unittest.TestCase):
    def make_pair(self, asm: str, c: str) -> tuple[Path, Path, Path, tempfile.TemporaryDirectory]:
        temporary = tempfile.TemporaryDirectory()
        root = Path(temporary.name)
        (root / "asm").mkdir()
        (root / "src" / "game").mkdir(parents=True)
        asm_path = root / "asm" / "TEST.ASM"
        c_path = root / "src" / "game" / "test.c"
        asm_path.write_text(asm, encoding="utf-8")
        c_path.write_text(
            "/* Source module: asm/TEST.ASM */\nvoid TEST(void) {\n" + c + "}\n",
            encoding="utf-8",
        )
        return root, asm_path, c_path, temporary

    def test_ignores_comments_directives_and_data(self) -> None:
        ignored = [
            "",
            "* developer comment",
            "; disabled instruction",
            '\t.FILE\t"TEST.ASM"',
            "\t.include\tMACS.EQU",
            "\t.float\t1.0",
        ]
        for line in ignored:
            with self.subTest(line=line):
                self.assertFalse(is_asm_code_line(line))

        self.assertTrue(is_asm_code_line("\tLDI\t1,R0"))
        self.assertTrue(is_asm_code_line("LOOP\tADDI\t1,R0"))

    def test_includes_code_labels_but_not_data_labels(self) -> None:
        root, asm_path, _c_path, temporary = self.make_pair(
            "TABLE:\n\t.float\t1.0\nFUNC:\n\t.if\tDEBUG\nLOOP\n\tADDI\t1,R0\n\t.endif\n\tRETS\n",
            "",
        )
        self.addCleanup(temporary.cleanup)
        lines = read_asm_code_lines(asm_path)
        self.assertEqual(
            [line.text for line in lines],
            ["FUNC:", "LOOP", "ADDI\t1,R0", "RETS"],
        )

    def test_matching_order_passes(self) -> None:
        root, asm_path, c_path, temporary = self.make_pair(
            "TEST:\n\tLDI\t1,R0\nLOOP\tADDI\t1,R0\t; increment\n\tRETS\n",
            "// asm: \tLDI\t1,R0\nint x = 1;\n"
            "// asm: LOOP\tADDI\t1,R0\t; increment\n"
            "// asm: \tRETS\n",
        )
        self.addCleanup(temporary.cleanup)
        self.assertEqual(compare_pair(c_path, asm_path, root), [])

    def test_reports_missing_and_extra_lines(self) -> None:
        root, asm_path, c_path, temporary = self.make_pair(
            "TEST:\n\tLDI\t1,R0\n\tADDI\t2,R0\n\tRETS\n",
            "// asm: \tLDI\t1,R0\n// asm: \tFAKE R0\n// asm: \tRETS\n",
        )
        self.addCleanup(temporary.cleanup)
        errors = compare_pair(c_path, asm_path, root)
        self.assertTrue(any("missing or out-of-order" in error and "ADDI" in error for error in errors))
        self.assertTrue(any("extra, invented" in error and "FAKE" in error for error in errors))

    def test_reports_reordered_lines(self) -> None:
        root, asm_path, c_path, temporary = self.make_pair(
            "TEST:\n\tLDI\t1,R0\n\tADDI\t2,R0\n\tRETS\n",
            "// asm: \tADDI\t2,R0\n// asm: \tLDI\t1,R0\n// asm: \tRETS\n",
        )
        self.addCleanup(temporary.cleanup)
        errors = compare_pair(c_path, asm_path, root)
        self.assertGreaterEqual(len(errors), 2)
        self.assertTrue(all("out-of-order" in error for error in errors))

    def test_source_module_marker_selects_pair(self) -> None:
        root, asm_path, c_path, temporary = self.make_pair("TEST:\n\tRETS\n", "// asm: \tRETS\n")
        self.addCleanup(temporary.cleanup)
        self.assertEqual(source_module_for_c(c_path, root), asm_path)

    def test_numbered_comments_and_c_labels_share_the_ordered_stream(self) -> None:
        root, asm_path, c_path, temporary = self.make_pair(
            "TEST:\n\tLDI\t1,R0\nLOOP\tSUBI\t1,R0\n\tBNZ\tLOOP\n\tRETS\n",
            "// asm 00000001: \tLDI\t1,R0\n"
            "LOOP:\n"
            "// asm 00000002: \tSUBI\t1,R0\n"
            "// asm 00000003: \tBNZ\tLOOP\n"
            "// asm 00000004: \tRETS\n",
        )
        self.addCleanup(temporary.cleanup)
        self.assertEqual(compare_pair(c_path, asm_path, root), [])

    def test_deduplicates_asm_label_comment_before_c_label(self) -> None:
        root, asm_path, c_path, temporary = self.make_pair(
            "TEST:\nLOOP\tSUBI\t1,R0\n\tRETS\n",
            "// asm 00000001: LOOP\n"
            "LOOP:\n"
            "// asm 00000001: \tSUBI\t1,R0\n"
            "// asm 00000002: \tRETS\n",
        )
        self.addCleanup(temporary.cleanup)
        self.assertEqual(compare_pair(c_path, asm_path, root), [])

    def test_translation_helper_can_own_a_shared_asm_tail(self) -> None:
        root, asm_path, c_path, temporary = self.make_pair(
            "TEST:\n\tLDI\t1,R0\nTAIL\tSUBI\t1,R0\n\tRETS\n",
            "// asm 00000001: \tLDI\t1,R0\n",
        )
        c_path.write_text(
            "/* Source module: asm/TEST.ASM */\n"
            "void TEST(void) {\n"
            "// asm 00000001: \tLDI\t1,R0\n"
            "}\n"
            "void TEST_tail(void) {\n"
            "TAIL:\n"
            "// asm 00000002: \tSUBI\t1,R0\n"
            "// asm 00000003: \tRETS\n"
            "}\n",
            encoding="utf-8",
        )
        self.addCleanup(temporary.cleanup)
        self.assertEqual(compare_pair(c_path, asm_path, root), [])


if __name__ == "__main__":
    unittest.main()
