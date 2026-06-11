# AGENTS

Cruis'n USA reverse-engineering workspace.

## Main Files

- `tools/ida/walk_source_and_rom.py`: main mapper; walks source + ROM listing and regenerates labels/import scripts.
- `tools/ida/shared_lib.py`: shared parsing/macro helpers.
- `tools/ida/test_romlst_mapper.py`: regression tests for mapper behavior.
- `tools/ida/address.map`: canonical persistent name -> address database.
- `tools/ida/ida_label_import.py`: generated IDAPython label import script.
- `tools/ida/log/`: generated TSV/log outputs.

## Sources Of Truth

- Source ASM/EQU in repo root.
- Current ROM listing:
  - `~/Downloads/carma/cruisin/crusnusa45_maindata_interleaved_bswap32.bin.lst`
- Canonical word ROM:
  - `roms/crusnusa45_maindata_interleaved_bswap32.bin`

## Normal Workflow

1. Update the `.lst` in Downloads if IDA code/data marking changed.
2. Run:
   - `python3 tools/ida/test_romlst_mapper.py`
   - `python3 tools/ida/walk_source_and_rom.py`
3. Apply:
   - `tools/ida/ida_label_import.py`

## Project Rules

- Prefer low ROM/FASTRAM pointer cells like `0x0000C67E`; high-page `_ptr_008*` / `_ptr_009*` labels are usually bad assumed-DP artifacts.
- Do not trust imported IDA names blindly; `address.map` is the persistent canonical map.
- `_ptr_...` labels come from `LDL` / `TEXTIT` literal cells.
- Global `.set` / `.globl` symbols should stay unscoped; non-globals use `name@MODULE`.
- Ignore labels ending in `?`.
- Branch labels matter; delayed branch forms (`BGTD`, `BNND`, etc.) should be treated like normal branch target producers.

## Notes

- `DP` mistakes in IDA can poison operand names in the `.lst`.
- The mapper currently leaves raw `.lst` literal-cell EAs visible; it does not auto-canonicalize `_ptr` addresses.
- If stale labels reappear, check whether they came from `address.map` or a newly generated `.lst`.
