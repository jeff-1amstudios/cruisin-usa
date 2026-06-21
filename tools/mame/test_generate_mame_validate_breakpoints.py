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
    parse_address_map,
    render_output,
)


def test_extracts_example_breakpoint() -> None:
    address_map = parse_address_map(ROOT / "tools" / "ida" / "address.map")
    entries = collect_breakpoints_for_file(ROOT / "src" / "game" / "cusa.c", address_map)
    first_wdhit = next(entry for entry in entries if entry.label == "WDHIT")
    assert first_wdhit.variable_name == "WDHIT"
    assert first_wdhit.next_instruction_address == 0x00004B3C
    assert first_wdhit.variable_address == 0x008099E1


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
        assert entries[0].format_mame() == 'bpset 00001234, 1, { logerror "VALUE: 0x%08X\\n", d@00800010; g }'

        rendered = render_output(entries)
        assert 'bpset 00001234, 1, { logerror "VALUE: 0x%08X\\n", d@00800010; quit }' in rendered


def test_ignores_commented_out_validate_call() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = pathlib.Path(tmpdir)
        sample_c = tmp / "sample.c"
        sample_c.write_text(
            textwrap.dedent(
                """
                LABEL:
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


def main() -> int:
    test_extracts_example_breakpoint()
    print("ok: real cusa.c WDHIT validate site resolves to 0x00004B3C / 0x008099E1")
    test_renders_expected_debugger_command()
    print("ok: synthetic sample renders expected MAME debugger command")
    test_ignores_commented_out_validate_call()
    print("ok: commented-out mame_validate_word lines are ignored")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
