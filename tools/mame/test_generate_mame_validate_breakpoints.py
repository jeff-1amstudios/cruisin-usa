#!/usr/bin/env python3
from __future__ import annotations

import pathlib
import sys
import tempfile
import textwrap

ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))

from mame.generate_mame_validate_breakpoints import (
    collect_breakpoints,
    collect_breakpoints_for_file,
    parse_constant_defines,
    parse_address_map,
    render_output,
)


def test_extracts_example_breakpoint() -> None:
    address_map = parse_address_map(ROOT / "tools" / "ida" / "address.map")
    entries = collect_breakpoints_for_file(
        ROOT / "src" / "game" / "cusa.c",
        address_map,
        parse_constant_defines(ROOT / "src"),
    )
    screen0 = next(entry for entry in entries if entry.label == "SCREEN0")
    assert screen0.variable_name == "SCREEN0"
    assert screen0.instruction_address == 0x00004B5E
    assert screen0.variable_address == 0x00900000
    assert screen0.array_length == 0x40000


def test_mame_validate_arg_uses_function_entry_and_register() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                uint32_t PAL_ALLOC_RAW(void* palette_source) {
                    mame_validate_arg("AR2", palette_source);
                    return 0;
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:00009F5A       PAL_ALLOC_RAW\n")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x00009F5A
        assert (
            entries[0].format_mame()
            == 'bpset 00009F5A, 1, { logerror "validate AR2: 0x%08X, sample.c:2\\n", ar2; g }'
        )


def test_mame_validate_arg_sym_uses_function_entry_and_register() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                uint32_t PAL_ALLOC_RAW(void* palette_source) {
                    mame_validate_arg_sym("AR2", palette_source);
                    return 0;
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:00009F5A       PAL_ALLOC_RAW\n")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x00009F5A
        assert (
            entries[0].format_mame()
            == 'bpset 00009F5A, 1, { logerror "validate AR2: 0x%08X, sample.c:2\\n", ar2; g }'
        )


def test_mame_validate_arg_float_uses_function_entry_and_float_register_label() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                uint32_t TEXT_ADDDS(const char* text, float x, float y, int ticks) {
                    mame_validate_arg_float("R3", &y);
                    return 0;
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:00007A81       TEXT_ADDDS\n")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x00007A81
        assert (
            entries[0].format_mame()
            == 'bpset 00007A81, 1, { logerror "validate R3F: 0x%08X, sample.c:2\\n", r3f; g }'
        )


def test_mame_validate_function_entry_uses_function_entry_and_name() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                uint32_t STRLEN(const char* text) {
                    MAME_VALIDATE_FUNCTION_ENTRY();
                    return 0;
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:00007AB9       STRLEN\n")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x00007AB9
        assert (
            entries[0].format_mame()
            == 'bpset 00007AB9, 1, { logerror "function STRLEN\\n"; g }'
        )


def test_mame_validate_exit_uses_function_entry_and_exit_action() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                uint32_t PAL_ALLOC_RAW(void* palette_source) {
                    MAME_VALIDATE_EXIT();
                    return 0;
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:00009F5A       PAL_ALLOC_RAW\n")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x00009F5A
        assert (
            entries[0].format_mame()
            == 'bpset 00009F5A, 1, { logerror "exit, sample.c:2\\n"; exit }'
        )


def test_mame_validate_reg_at_addr_uses_explicit_breakpoint() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                void test(uint32_t palette_code) {
                    mame_validate_reg_at_addr(0x00009F86, "R0", &palette_code);
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text("")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x00009F86
        assert (
            entries[0].format_mame()
            == 'bpset 00009F86, 1, { logerror "validate R0: 0x%08X, sample.c:2\\n", r0; g }'
        )


def test_mame_validate_reg_at_addr_accepts_indexed_address_expr() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                uint32_t _AIVI[4];

                void test(void) {
                    mame_validate_reg_at_addr(0x00007AB2, "R0", &_AIVI[0]);
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text("")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x00007AB2
        assert (
            entries[0].format_mame()
            == 'bpset 00007AB2, 1, { logerror "validate R0: 0x%08X, sample.c:4\\n", r0; g }'
        )


def test_mame_validate_reg_at_addr_accepts_pointer_member_expr() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                typedef struct Obj {
                    int radius;
                } Obj;

                void test(Obj* obj) {
                    mame_validate_reg_at_addr(0x0000703C, "R0", &obj->radius);
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text("")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x0000703C
        assert entries[0].variable_name == "obj->radius"
        assert (
            entries[0].format_mame()
            == 'bpset 0000703C, 1, { logerror "validate R0: 0x%08X, sample.c:6\\n", r0; g }'
        )


def test_mame_validate_reg_at_addr_float_uses_float_register_label() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                void test(float x) {
                    mame_validate_reg_at_addr_float(0x00007A81, "R2", &x);
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text("")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x00007A81
        assert (
            entries[0].format_mame()
            == 'bpset 00007A81, 1, { logerror "validate R2F: 0x%08X, sample.c:2\\n", r2f; g }'
        )


def test_mame_validate_reg_at_addr_float_accepts_pointer_member_expr() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                typedef struct Obj {
                    float radius;
                } Obj;

                void test(Obj* obj) {
                    mame_validate_reg_at_addr_float(0x0000703C, "R0", &obj->radius);
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text("")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x0000703C
        assert entries[0].variable_name == "obj->radius"
        assert (
            entries[0].format_mame()
            == 'bpset 0000703C, 1, { logerror "validate R0F: 0x%08X, sample.c:6\\n", r0f; g }'
        )


def test_mame_validate_region_at_addr_uses_explicit_breakpoint_and_region() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_h = tmp / "sample.h"
        sample_h.write_text("#define WORDS 16\n")
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                #include "sample.h"

                void test(void) {
                    mame_validate_region_at_addr(0x00009F90, "SCREEN0", SCREEN0, g_crusn_machine->screen_words, WORDS);
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:00900000       SCREEN0\n")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].instruction_address == 0x00009F90
        assert entries[0].variable_address == 0x00900000
        assert entries[0].array_length == 16
        assert (
            entries[0].format_mame()
            == 'bpset 00009F90, 1, { save 00009F90-0.bin, 00900000, 0x10; logerror "validate SCREEN0: file=00009F90-0.bin, sample.c:4\\n"; g }'
        )


def test_mame_validate_region_at_addr_groups_with_other_validations() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_h = tmp / "sample.h"
        sample_h.write_text("#define WORDS 8\n")
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                #include "sample.h"

                int VALUE;

                void test(void) {
                    mame_validate_region_at_addr(0x00009F90, "SCREEN0", SCREEN0, g_crusn_machine->screen_words, WORDS);
                    mame_validate_reg_at_addr(0x00009F90, "R0", &VALUE);
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:00900000       SCREEN0\n")

        rendered = render_output(collect_breakpoints(tmp, parse_address_map(sample_map)))
        assert rendered.count("bpset 00009F90, 1, {") == 1
        assert (
            'bpset 00009F90, 1, { save 00009F90-0.bin, 00900000, 0x8; logerror "validate SCREEN0: file=00009F90-0.bin, sample.c:6\\n"; '
            'logerror "validate R0: 0x%08X, sample.c:7\\n", r0; g }'
        ) in rendered


def test_same_breakpoint_address_preserves_source_line_order() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                void test(uint32_t t) {
                    mame_validate_reg_at_addr(0x00004B89, "R0", &t);
                    mame_validate_reg_at_addr(0x00004B89, "HARD_SECTION_LOAD", &t);
                    mame_validate_region_at_addr(0x00004B89, "_SECshared-compressed", 0x0D00000, ROM_PTR(0x0D00000), 1000);
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text("")

        rendered = render_output(collect_breakpoints(tmp, parse_address_map(sample_map)))
        assert (
            'bpset 00004B89, 1, { logerror "validate R0: 0x%08X, sample.c:2\\n", r0; '
            'logerror "validate HARD_SECTION_LOAD: 0x%08X, sample.c:3\\n", hard_section_load; '
            'save 00004B89-0.bin, 00D00000, 0x3E8; logerror "validate _SECshared-compressed: file=00004B89-0.bin, sample.c:4\\n"; '
            'g }'
        ) in rendered


def test_function_entry_groups_before_other_function_entry_validations() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                uint32_t SET_SOMETHING(uint32_t value) {
                    MAME_VALIDATE_FUNCTION_ENTRY();
                    mame_validate_arg("R0", &value);
                    return value;
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:00001234       SET_SOMETHING\n")

        rendered = render_output(collect_breakpoints(tmp, parse_address_map(sample_map)))
        assert (
            'bpset 00001234, 1, { logerror "function SET_SOMETHING\\n"; '
            'logerror "validate R0: 0x%08X, sample.c:3\\n", r0; g }'
        ) in rendered


def main() -> int:
    test_extracts_example_breakpoint()
    print("ok: real cusa.c SCREEN0 validate site resolves to 0x00004B5E / 0x00900000")
    test_mame_validate_arg_uses_function_entry_and_register()
    print("ok: mame_validate_arg emits a function-entry register breakpoint")
    test_mame_validate_arg_sym_uses_function_entry_and_register()
    print("ok: mame_validate_arg_sym emits a function-entry register breakpoint")
    test_mame_validate_arg_float_uses_function_entry_and_float_register_label()
    print("ok: mame_validate_arg_float emits a function-entry float register breakpoint")
    test_mame_validate_function_entry_uses_function_entry_and_name()
    print("ok: MAME_VALIDATE_FUNCTION_ENTRY emits a function-entry name breakpoint")
    test_mame_validate_exit_uses_function_entry_and_exit_action()
    print("ok: MAME_VALIDATE_EXIT emits a function-entry exit breakpoint")
    test_mame_validate_reg_at_addr_uses_explicit_breakpoint()
    print("ok: mame_validate_reg_at_addr emits an explicit-address register breakpoint")
    test_mame_validate_reg_at_addr_accepts_indexed_address_expr()
    print("ok: mame_validate_reg_at_addr accepts indexed address expressions")
    test_mame_validate_reg_at_addr_float_uses_float_register_label()
    print("ok: mame_validate_reg_at_addr_float emits an explicit-address float register breakpoint")
    test_mame_validate_region_at_addr_uses_explicit_breakpoint_and_region()
    print("ok: mame_validate_region_at_addr emits an explicit-address region dump")
    test_mame_validate_region_at_addr_groups_with_other_validations()
    print("ok: mame_validate_region_at_addr groups with other validations at the same address")
    test_same_breakpoint_address_preserves_source_line_order()
    print("ok: same-address validations preserve source line ordering")
    test_function_entry_groups_before_other_function_entry_validations()
    print("ok: function-entry logging groups before later function-entry validations")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
