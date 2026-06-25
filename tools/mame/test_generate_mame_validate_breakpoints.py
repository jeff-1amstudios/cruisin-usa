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
        {},
        parse_constant_defines(ROOT / "src"),
    )
    screen0 = next(entry for entry in entries if entry.label == "SCREEN0")
    assert screen0.variable_name == "SCREEN0"
    assert screen0.instruction_address == 0x00004B5E
    assert screen0.variable_address == 0x00900000
    assert screen0.array_length == 0x40000


def test_renders_expected_debugger_command() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                LABEL:
                    VALUE = R0.s;
                    mame_validate_word("VALUE", &VALUE);

                NEXT:
                    // asm 00001234:   LDI 1,R0
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:00800010       VALUE\n")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].format_mame() == 'bpset 00001234, 1, { logerror "validate VALUE: 0x%08X\\n", d@00800010; g }'

        rendered = render_output(entries)
        assert 'bpset 00001234, 1, { logerror "validate VALUE: 0x%08X\\n", d@00800010; g }' in rendered


def test_small_array_uses_printf_once() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                int SWRAM[3];

                void test(void) {
                    mame_validate_word("SWRAM", &SWRAM[0]);
                    mame_validate_word("SWRAM+1", &SWRAM[1]);
                    mame_validate_word("SWRAM+2", &SWRAM[2]);
                    // asm 00002002:   LDI 1,R0
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:00800100       SWRAM\n")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].array_length == 3
        assert (
            entries[0].format_mame()
            == 'bpset 00002002, 1, { logerror "validate SWRAM[3]: 0x%08X 0x%08X 0x%08X\\n", d@00800100, d@00800101, d@00800102; g }'
        )


def test_large_array_uses_save() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                int RAWLOCS[128];

                void test(void) {
                    mame_validate_word("RAWLOCS", &RAWLOCS[0]);
                    // asm 00009F80:   LSH 8,R0
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(" 0000:0000CA3D       RAWLOCS\n")

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].array_length == 128
        rendered = render_output(entries)
        assert (
            'bpset 00009F80, 1, { save 00009F80-0.bin, 0000CA3D, 0x80; '
            'logerror "validate RAWLOCS: file=00009F80-0.bin\\n"; g }'
        ) in rendered


def test_shared_hook_address_is_collapsed() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                int PTTRAM[384];
                int PALRAM[128];
                int RAWLOCS[128];
                int _PALLIST[256];

                void test(void) {
                    mame_validate_word("PTTRAM", &PTTRAM);
                    mame_validate_word("PALRAM", &PALRAM);
                    mame_validate_word("RAWLOCS", &RAWLOCS);
                    mame_validate_word("_PALLIST", &_PALLIST);
                    // asm 00009EBE:   CALL PALXFER_INIT
                }
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(
            " 0000:0000CABD       PTTRAM\n"
            " 0000:0000C9BD       PALRAM\n"
            " 0000:0000CA3D       RAWLOCS\n"
            " 0000:0000EA7C       _PALLIST\n"
        )

        rendered = render_output(collect_breakpoints(tmp, parse_address_map(sample_map)))
        assert rendered.count("bpset 00009EBE, 1, {") == 1
        assert (
            'bpset 00009EBE, 1, { '
            'save 00009EBE-0.bin, 0000CABD, 0x180; logerror "validate PTTRAM: file=00009EBE-0.bin\\n"; '
            'save 00009EBE-1.bin, 0000C9BD, 0x80; logerror "validate PALRAM: file=00009EBE-1.bin\\n"; '
            'save 00009EBE-2.bin, 0000CA3D, 0x80; logerror "validate RAWLOCS: file=00009EBE-2.bin\\n"; '
            'save 00009EBE-3.bin, 0000EA7C, 0x100; logerror "validate _PALLIST: file=00009EBE-3.bin\\n"; '
            'g }'
        ) in rendered


def test_ignores_commented_out_validate_call() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                LABEL:
                    // asm 00001233:   STI R0,@ACTIVE
                    ACTIVE = R0.s;
                    mame_validate_word("ACTIVE", &ACTIVE);
                    // mame_validate_word("IGNORED", &IGNORED);

                NEXT1:
                    // asm 00001234:   LDI 1,R0

                NEXT2:
                    // asm 00001235:   LDI 2,R0
                """
            ).strip()
            + "\n"
        )
        sample_map = tmp / "address.map"
        sample_map.write_text(
            " 0000:00800010       ACTIVE\n"
            " 0000:00800020       IGNORED\n"
        )

        entries = collect_breakpoints(tmp, parse_address_map(sample_map))
        assert len(entries) == 1
        assert entries[0].label == "ACTIVE"


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
        assert entries[0].format_mame() == 'bpset 00009F5A, 1, { logerror "validate AR2: 0x%08X\\n", ar2; g }'


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
        assert entries[0].format_mame() == 'bpset 00009F86, 1, { logerror "validate R0: 0x%08X\\n", r0; g }'


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
            == 'bpset 00009F90, 1, { save 00009F90-0.bin, 00900000, 0x10; logerror "validate SCREEN0: file=00009F90-0.bin\\n"; g }'
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
            'bpset 00009F90, 1, { save 00009F90-0.bin, 00900000, 0x8; logerror "validate SCREEN0: file=00009F90-0.bin\\n"; '
            'logerror "validate R0: 0x%08X\\n", r0; g }'
        ) in rendered


def main() -> int:
    test_extracts_example_breakpoint()
    print("ok: real cusa.c SCREEN0 validate site resolves to 0x00004B5E / 0x00900000")
    test_renders_expected_debugger_command()
    print("ok: synthetic sample renders expected MAME debugger command")
    test_small_array_uses_printf_once()
    print("ok: small arrays render a single logerror breakpoint")
    test_large_array_uses_save()
    print("ok: large arrays render save plus filename logerror")
    test_shared_hook_address_is_collapsed()
    print("ok: shared hook addresses render as one grouped breakpoint")
    test_ignores_commented_out_validate_call()
    print("ok: commented-out mame_validate_word lines are ignored")
    test_mame_validate_arg_uses_function_entry_and_register()
    print("ok: mame_validate_arg emits a function-entry register breakpoint")
    test_mame_validate_reg_at_addr_uses_explicit_breakpoint()
    print("ok: mame_validate_reg_at_addr emits an explicit-address register breakpoint")
    test_mame_validate_region_at_addr_uses_explicit_breakpoint_and_region()
    print("ok: mame_validate_region_at_addr emits an explicit-address region dump")
    test_mame_validate_region_at_addr_groups_with_other_validations()
    print("ok: mame_validate_region_at_addr groups with other validations at the same address")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
