# Building Original ASM on macOS (Investigation Notes)

## What the current build expects

The top-level `MAKEFILE` is for an old DOS-based TI C30 toolchain and expects these commands:

- `asm30` (assembler)
- `cl30` (C compiler for `line.c` and `hpmath.c`)
- `lnk30` (linker)
- `strpul`, `sndc`, `pcomp`, `tv30`, `mkmf`, `mkdate`, plus DOS shell commands (`copy`, `del`, `move`)

It also assumes:

- DOS drive letters (`WDIR=e:`)
- case-insensitive filesystem
- DOS `NUL` redirection and command semantics

## Current blockers in this repo

1. Toolchain binaries are not present (`asm30`, `cl30`, `lnk30`, etc.).
2. The GNU make default lookup misses uppercase `MAKEFILE` unless `-f MAKEFILE` is used.
3. Many dependency files referenced by the makefile are missing in this checkout, e.g.:
   - `babe.pal`, `caravan.pal`, `cow.pal`, `cvette.pal`, `dnums2.pal`, `engine.pal`, `font18.pal`, `font43.pal`, `glight.pal`, `gtruck.pal`, `hotrod.pal`, `jeep.pal`, `light.pal`, `missle.pal`, `muscle.pal`, `ommdfont.pal`, `osg10fnt.pal`, `plate.pal`, `ptruckg.pal`, `rpm.pal`, `scroll.pal`, `smoke.pal`, `testor.pal`
4. The pipeline appears to rely on asset-generation tools (`pcomp`, `sndc`) that are also absent.

Because of (1) and (3), we cannot produce `cusa.out` yet even with emulation.

## Viable macOS strategies

### Recommended: DOSBox-X on macOS

Best compatibility for old DOS executables and DOS batch/make behavior.

High-level flow:

1. Install DOSBox-X.
2. Prepare a DOS tools directory with TI C30 binaries and helper tools.
3. Mount project as a DOS drive (`E:`) in DOSBox-X.
4. Use a DOS make utility compatible with this makefile, or run the equivalent build command sequence manually.
5. Ensure missing `.pal` assets are generated or restored before link.

Why this is likely best:

- Native DOS command behavior (`copy`, `del`, `NUL`) matches the makefile.
- Drive-letter expectations (`e:`) map naturally.

### Possible: Docker + DOSBox-X (headless/scripted)

This can work on Apple Silicon via `linux/amd64` containers, but setup is more complex than running DOSBox-X directly on macOS.

Use when you want reproducibility/CI-style environment after first local success.

### Lower confidence: Wine-only path

Possible for some 16-bit Windows-era binaries, but pure DOS build tooling is less reliable under Wine than under DOSBox-style environments.

Treat Wine as fallback if (and only if) your specific TI tools prove to be Windows executables that run cleanly there.

## Practical next step plan

1. Collect required legacy tools (`asm30`, `cl30`, `lnk30`, `pcomp`, `sndc`, `strpul`, `tv30`, and DOS `make` used by the team).
2. Restore or regenerate missing `.pal` inputs.
3. Run first build inside DOSBox-X with `E:` mapped to this repo.
4. After first successful build, optionally containerize that exact process (Docker + DOSBox-X) for reproducibility.

## References

- TI C3x/C4x Assembly Language Tools guide (documents `asm30`, `lnk30` usage):
  - https://www.ti.com/lit/ug/spru119b/spru119b.pdf
- TI C3x/C4x C compiler docs (`cl30` toolchain references):
  - https://www.ti.com/lit/ug/spru034h/spru034h.pdf
- Docker Desktop on Mac:
  - https://docs.docker.com/installation/mac/
- Docker multi-platform (amd64 on arm64 context):
  - https://docs.docker.com/build/building/multi-platform/
- DOSBox-X command and mount docs:
  - https://dosbox-x.com/wiki/DOSBox%E2%80%90X%E2%80%99s-Supported-Commands
  - https://dosbox-x.com/wiki/Home
- Wine 11.0 release notes (WoW64/16-bit statements):
  - https://list.winehq.org/archives/list/wine-releases@list.winehq.org/thread/UL6L2GJ55VYUJ5KUMBZ3TZSXRFJ52QG6/
