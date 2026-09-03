#!/usr/bin/env bash
set -e

: "${CRUSN_VALIDATE_SINGLE_FRAME:=1}"
: "${CRUSN_VALIDATE_CLEAR_WATER_R0:=1}"
: "${CRUSN_VALIDATE_ALIGN_STOPWATCH_PHASE:=0}"
: "${CRUSN_VALIDATE_ALIGN_SECTIME_PHASE:=1}"
: "${CRUSN_VALIDATE_SKIP_ATTRACT:=1}"
: "${CRUSN_VALIDATE_FORCE_BONUS_START:=1}"
export CRUSN_VALIDATE_SINGLE_FRAME
export CRUSN_VALIDATE_CLEAR_WATER_R0
export CRUSN_VALIDATE_ALIGN_STOPWATCH_PHASE
export CRUSN_VALIDATE_ALIGN_SECTIME_PHASE
export CRUSN_VALIDATE_SKIP_ATTRACT
export CRUSN_VALIDATE_FORCE_BONUS_START

python3 tools/mame/generate_mame_validate_breakpoints.py
# clear out to have clean run each time - reset high score tables etc
# rm -r ~/Library/Application\ Support/mame/nvram/crusnusa

# Run from a clean boot until MAME exits. The debugger script redirects the
# initial attract wave into the normal game/track-selection path.
/opt/homebrew/Cellar/mame/0.287/bin/mame crusnusa -window -sound none \
    -debug -log -skip_gameinfo \
    -seconds_to_run 200 \
    -resolution 640x480 \
    -debugscript tools/mame/output/mame_validate_breakpoints.txt
#mame crusnusa  -sound none -debug -skip_gameinfo
mkdir -p mame_validate
mv error.log mame_validate/mame.log
mv *.bin mame_validate || true
