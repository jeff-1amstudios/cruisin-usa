# Cruis'n USA

A work-in-progress port of the original [Cruis'n USA](https://en.wikipedia.org/wiki/Cruis%27n_USA) arcade game from
[TMS320C30](https://en.wikipedia.org/wiki/TMS320) assembly to portable C using SDL2.

The goal is **100% functional equivalence with the version 4.5 arcade ROM** on
modern systems.

<p align="center">
  <img src="docs/images/cruisin-usa-port.png" alt="Cruis'n USA running in the SDL port" width="900">
</p>

## Status

The port currently plays the attract sequence shown above. It is under active
development and is not yet fully playable.

## Background

The original assembly source was published by
[Historical Source](https://github.com/historicalsource/cruisin-usa) and is
preserved in [`asm/`](asm/). That source is **version 4.4**, while this project
targets the later **version 4.5** retail ROM.

To recover the changes made between those versions, this project uses a
[source ↔ ROM disassembly walker](tools/ida/walk_source_and_rom.py). It aligns
the 4.4 source with the disassembled 4.5 program, allowing the code and data
that actually shipped to be identified and restored.

- [4.4 to 4.5 changes](https://github.com/jeff-1amstudios/cruisin-usa/commit/2033b4e68533fe9055101752713465132aad751e)

## Asset recovery
The original archive also omits game assets and their final IDs and
memory locations. The ROM walker recovers these from matched instructions. For
example:
1. We start with asm code that references an unknown value. [(link)](https://github.com/jeff-1amstudios/cruisin-usa/blob/643868980449fae3f40b2cad4e4422eebe4e9343/asm/BONUS.ASM#L665)
```
...
CALL  KILL_THE_REANIMATORS		;cheering crowd
LDL	  shared_PALETTES,AR2
CALL  dealloc_section
...
```

2. The walker matches that code to `0x00003AF4` in the 4.5 ROM so now we have
```
CODE:00003AF4    LDL	shared_PALETTES,AR2
```
3. At `00003AF4`, the disassembly shows us that the value being set on AR2 is a pointer to `0x0000A16A` so we know that `shared_PALETTES` should be defined as a pointer to `0x0000A16A` in the ROM and can now reference it in our ported C code.

The same process recovers asset IDs, palette numbers, function addresses, memory locations, and other missing constants.

## Workflow

AI is used to translate the game function by function, keeping the original
assembly beside the generated C for review and debugging.

Correctness is the key challenge on a project like this. Its "easy(!?)" enough to generate reasonable looking code, but much harder to know if it is really correct or not.

In other c-based decomp projects, assembly-level correctness can be used, like https://github.com/dethrace-labs/dethrace or https://github.com/isledecomp/isle. Here, that is not possible as the original code is written in TMS320C assembly, not c.

Instead, this project uses **MAME** running version 4.5 as the source of truth and checks behavior against it at runtime.
Assertion markers capture function entries, registers, and memory from MAME;
the port then replays the same sequence and stops at the first difference. See
the [translation guide](docs/function-translation.md) and
[debugging guide](docs/debugging.md) for details.

This enables us to run a tight loop of 
- writing code
- asserting its behavior
- observing differences vs MAME
- patching
- re-observing and repeat until it all matches.

## Building

Dependencies:

- CMake 3.20+
- SDL2
- A C11 compiler
- Python 3

```sh
cmake -S . -B build
cmake --build build
```

To check translated C3x floating-point operations:

```sh
cmake --build build --target check-c3x-translation
```

## Running

This project does not include arcade ROMs or missing game assets. Supply a
legally obtained version 4.5 main-data image at:

```text
roms/crusnusa45_maindata_interleaved.bin
```

Run the game from the repository root. Free play is enabled by default and MAME
validation is disabled:

```sh
./build/crusn
```

Use `--no-free-play` to use the normal coin settings. Press <kbd>Return</kbd> for
Start.

## Contributing

Correctness takes priority over cleanup. Keep translations close to the
original assembly and validate behavior against MAME wherever practical. Start
with the [function translation guide](docs/function-translation.md); coroutine
style `PROC` routines also have a [dedicated guide](docs/proc-translation.md).

## Legal

*Cruis'n USA* and its original code and assets belong to their respective
rights holders. This is an independent preservation and compatibility project
and is not affiliated with or endorsed by the original developers or
publishers. No commercial ROM data is included.
