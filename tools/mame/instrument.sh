#!/usr/bin/env bash
set -e

python3 tools/mame/generate_mame_validate_breakpoints.py
mame crusnusa -window -seconds_to_run 15 -sound none -debug -log -skip_gameinfo -debugscript tools/mame/output/mame_validate_word_breakpoints.txt
#mame crusnusa  -video none -sound none -seconds_to_run 20 -debug -log -skip_gameinfo -debugscript tools/mame/output/mame_validate_word_breakpoints.txt
mkdir -p mame_validate
mv error.log mame_validate/mame.log
mv *.bin mame_validate
