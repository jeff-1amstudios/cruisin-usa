#!/usr/bin/env python3

from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock


TOOLS_DIR = Path(__file__).resolve().parent
if str(TOOLS_DIR) not in sys.path:
    sys.path.insert(0, str(TOOLS_DIR))

import check_c3x_translation as checker


class C3xTranslationCheckTests(unittest.TestCase):
    def check_source(self, source: str) -> list[str]:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            path = root / "src" / "game" / "sample.c"
            path.parent.mkdir(parents=True)
            path.write_text(source, encoding="utf-8")
            errors: list[str] = []
            with mock.patch.object(checker, "ROOT", root):
                checker.check_file(path, errors)
            return errors

    def test_immediate_helpers_accept_raw_immediate_operands(self) -> None:
        errors = self.check_source(
            """
            void sample(c3x_reg_t value) {
                value = C3X_MUL_IMM(value, 0.5);
                value = C3X_RSUB_IMM(1.0, value);
                if (C3X_GT_IMM(value, 0.0)) {}
                c3x_f32_t stored = C3X_STF_IMM(2.5);
            }
            """
        )
        self.assertEqual(errors, [])

    def test_generic_arithmetic_rejects_raw_float(self) -> None:
        errors = self.check_source(
            """
            void sample(c3x_reg_t value) {
                value = C3X_MUL(value, 0.5);
            }
            """
        )
        self.assertEqual(len(errors), 1)
        self.assertIn("use the operation's _IMM form", errors[0])

    def test_symbolic_asm_immediate_accepts_immediate_helper(self) -> None:
        errors = self.check_source(
            """
            void sample(c3x_reg_t value) {
                // asm: MPYF HALFPI,R0
                value = C3X_MUL_IMM(value, HALFPI);
            }
            """
        )
        self.assertEqual(errors, [])

    def test_deprecated_raw_conversion_name_is_rejected(self) -> None:
        errors = self.check_source(
            """
            void sample(void) {
                c3x_reg_t value = C3X_LOAD(0x12345678u);
            }
            """
        )
        self.assertEqual(len(errors), 1)
        self.assertIn("use C3X_FROM_RAW32 instead of C3X_LOAD", errors[0])

if __name__ == "__main__":
    unittest.main()
