#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BREAKPOINT_FILE="$ROOT_DIR/tools/mame/output/mame_validate_breakpoints.txt"
BREAKPOINT_HASH_FILE="$ROOT_DIR/tools/mame/output/mame_validate_breakpoints.md5"
BUILD_DIR="$ROOT_DIR/build"
GAME_BIN="$BUILD_DIR/crusn"

: "${CRUSN_VALIDATE_SINGLE_FRAME:=1}"
: "${CRUSN_VALIDATE_CLEAR_WATER_R0:=1}"
: "${CRUSN_VALIDATE_START_FUNCTION:=}"
: "${CRUSN_VALIDATE_START_ORDERING:=RACE_TIMING_START}"
: "${CRUSN_VALIDATE_START_ORDERING_OCCURRENCE:=1}"
: "${CRUSN_VALIDATE_ALIGN_BGD_PHASE:=0}"
: "${CRUSN_VALIDATE_ALIGN_STOPWATCH_PHASE:=1}"
: "${CRUSN_VALIDATE_ALIGN_SECTIME_PHASE:=1}"
: "${CRUSN_VALIDATE_FORCE_POT1_MAX:=1}"
: "${CRUSN_VALIDATE_SCRIPT_CAR_CHOICE:=1}"
: "${CRUSN_VALIDATE_SKIP_ATTRACT:=1}"
export CRUSN_VALIDATE_SINGLE_FRAME
export CRUSN_VALIDATE_CLEAR_WATER_R0
export CRUSN_VALIDATE_START_FUNCTION
export CRUSN_VALIDATE_START_ORDERING
export CRUSN_VALIDATE_START_ORDERING_OCCURRENCE
export CRUSN_VALIDATE_ALIGN_BGD_PHASE
export CRUSN_VALIDATE_ALIGN_STOPWATCH_PHASE
export CRUSN_VALIDATE_ALIGN_SECTIME_PHASE
export CRUSN_VALIDATE_FORCE_POT1_MAX
export CRUSN_VALIDATE_SCRIPT_CAR_CHOICE
export CRUSN_VALIDATE_SKIP_ATTRACT
export CRUSN_ENABLE_MAME_VALIDATION=1

hash_file() {
    local file_path="$1"

    if command -v md5 >/dev/null 2>&1; then
        md5 -q "$file_path"
        return
    fi

    if command -v md5sum >/dev/null 2>&1; then
        md5sum "$file_path" | awk '{print $1}'
        return
    fi

    echo "No md5 tool found (expected md5 or md5sum)" >&2
    exit 1
}

mkdir -p "$(dirname "$BREAKPOINT_FILE")"

cd "$ROOT_DIR"

python3 tools/mame/generate_mame_validate_breakpoints.py

current_hash="$(hash_file "$BREAKPOINT_FILE")"
previous_hash=""

if [[ -f "$BREAKPOINT_HASH_FILE" ]]; then
    previous_hash="$(tr -d '[:space:]' < "$BREAKPOINT_HASH_FILE")"
fi

if [[ "$current_hash" != "$previous_hash" ]]; then
    echo "Breakpoint script changed; refreshing MAME capture"
    tools/mame/instrument.sh
    printf '%s\n' "$current_hash" > "$BREAKPOINT_HASH_FILE"
else
    echo "Breakpoint script unchanged; skipping MAME capture refresh"
fi

/opt/homebrew/bin/cmake -S "$ROOT_DIR" -B "$BUILD_DIR"
/opt/homebrew/bin/cmake --build "$BUILD_DIR"

# SDL_VIDEODRIVER=dummy \
# SDL_AUDIODRIVER=dummy \
# SDL_RENDER_DRIVER=software \
# "$GAME_BIN"

"$GAME_BIN" --no-sound
