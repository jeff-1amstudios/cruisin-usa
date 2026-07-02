#!/usr/bin/env bash
set -e

python3 tools/mame/generate_mame_validate_breakpoints.py
mame crusnusa -window -seconds_to_run 15 -sound none -debug -log -skip_gameinfo -debugscript tools/mame/output/mame_validate_breakpoints.txt
#mame crusnusa  -sound none -debug -skip_gameinfo
mkdir -p mame_validate
mv error.log mame_validate/mame.log
mv *.bin mame_validate
