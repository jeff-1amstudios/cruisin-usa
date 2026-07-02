# Cruisin USA arcade port debugging

We are translating the original TMS320C30 asm source code into portable c. We're now debugging functionality that you translated from the original .ASM files to .c

I have set up validation so we know when our implementation diverges from the expected values as produced by MAME. (`core/validator`).

You can build and run with `SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy SDL_RENDER_DRIVER=software`. When it fails, look at the output and it will tell you something like `/Users/j.harris/code/cruisin-usa/src/game/comp.c:189: validate_word(R0) failed: value mismatch; expected 0x00000050 but was 0x00000059`


## Manual hand over
You can add extra debugging printf lines to trace the C code but if you need to add or edit a mame_validate(...) call, you'll have to stop and ask me to re-run MAME to generate a new mame.log to run against.

I'll now give you more context on the specific bug.
