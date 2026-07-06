#!/usr/bin/env bash
set -e

python3 tools/mame/generate_mame_validate_breakpoints.py
# clear out to have clean run each time - reset high score tables etc
# rm -r ~/Library/Application\ Support/mame/nvram/crusnusa

mame crusnusa -window -sound none -debug -log -skip_gameinfo -debugscript tools/mame/output/mame_validate_breakpoints.txt
#mame crusnusa  -sound none -debug -skip_gameinfo
mkdir -p mame_validate
mv error.log mame_validate/mame.log
mv *.bin mame_validate
