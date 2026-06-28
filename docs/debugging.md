# Cruisin USA arcade port debugging

We are translating the original TMS320C30 asm source code into portable c. We're now debugging functionality that you translated from the original .ASM files to .c

I have set up validation so we know when our implementation diverges from the expected values as produced by MAME. (`core/validator`).

You can build and run with `SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy`. When it fails, look at the output and it will tell you something like `/Users/j.harris/code/cruisin-usa/src/game/comp.c:189: validate_word(R0) failed: value mismatch; expected 0x00000050 but was 0x00000059`

You can add extra debugging lines to trace the C code, but if you absolutely need more expected assertions from MAME, you'll have to stop and ask me to provide it.

I'll now give you more context on the specific bug.
