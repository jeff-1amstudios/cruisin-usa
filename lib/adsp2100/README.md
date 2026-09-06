# ADSP-21xx interpreter

This is a standalone BSD-3-Clause version of Aaron Giles's ADSP-21xx
interpreter. It retains the host-memory hooks needed by the Cruis'n USA DCS
board wrapper. The instruction core originated in MAME and is vendored here in
standalone form.

The port-specific DCS memory map, ROM banking, command latch, interrupt, and
SPORT autobuffer integration live in src/core/dcs_board.cpp.
