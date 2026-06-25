#!/usr/bin/env bash
set -e

python3 tools/mame/generate_mame_validate_breakpoints.py
mame crusnusa -window -sound none -debug -log -skip_gameinfo -debugscript tools/mame/output/mame_validate_word_breakpoints.txt
