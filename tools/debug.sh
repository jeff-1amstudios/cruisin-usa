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
: "${CRUSN_VALIDATE_ALIGN_BGD_PHASE:=1}"
: "${CRUSN_VALIDATE_ALIGN_STOPWATCH_PHASE:=0}"
: "${CRUSN_VALIDATE_ALIGN_SECTIME_PHASE:=1}"
: "${CRUSN_VALIDATE_SCRIPT_CAR_CHOICE:=1}"
: "${CRUSN_VALIDATE_SKIP_ATTRACT:=1}"
: "${CRUSN_VALIDATE_FORCE_BONUS_START:=1}"
: "${CRUSN_VALIDATE_EXIT_ON_LOG_END:=1}"
: "${CRUSN_VALIDATE_SECONDS_TO_RUN:=120}"
: "${CRUSN_VALIDATE_REALTIME:=0}"
export CRUSN_VALIDATE_SINGLE_FRAME
export CRUSN_VALIDATE_CLEAR_WATER_R0
export CRUSN_VALIDATE_START_FUNCTION
export CRUSN_VALIDATE_START_ORDERING
export CRUSN_VALIDATE_START_ORDERING_OCCURRENCE
export CRUSN_VALIDATE_ALIGN_BGD_PHASE
export CRUSN_VALIDATE_ALIGN_STOPWATCH_PHASE
export CRUSN_VALIDATE_ALIGN_SECTIME_PHASE
export CRUSN_VALIDATE_SCRIPT_CAR_CHOICE
export CRUSN_VALIDATE_SKIP_ATTRACT
export CRUSN_VALIDATE_FORCE_BONUS_START
export CRUSN_VALIDATE_EXIT_ON_LOG_END
export CRUSN_VALIDATE_SECONDS_TO_RUN
export CRUSN_VALIDATE_REALTIME
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

current_hash="$(hash_file "$BREAKPOINT_FILE")-$(hash_file "$ROOT_DIR/tools/mame/instrument.sh")-$CRUSN_VALIDATE_SECONDS_TO_RUN"
previous_hash=""

if [[ -f "$BREAKPOINT_HASH_FILE" ]]; then
    previous_hash="$(tr -d '[:space:]' < "$BREAKPOINT_HASH_FILE")"
fi

if [[ "$current_hash" != "$previous_hash" ]]; then
    echo "MAME capture inputs changed; refreshing MAME capture"
    tools/mame/instrument.sh
    printf '%s\n' "$current_hash" > "$BREAKPOINT_HASH_FILE"
else
    echo "Breakpoint script unchanged; skipping MAME capture refresh"
fi

/opt/homebrew/bin/cmake -S "$ROOT_DIR" -B "$BUILD_DIR"
/opt/homebrew/bin/cmake --build "$BUILD_DIR"

if [[ "$CRUSN_VALIDATE_REALTIME" == "0" ]]; then
    export SDL_VIDEODRIVER=dummy
    export SDL_AUDIODRIVER=dummy
    export SDL_RENDER_DRIVER=software
fi

if [[ "${CRUSN_DEBUG_NO_LLDB:-0}" == "1" ]]; then
    "$GAME_BIN" --no-sound --window
else
    lldb --batch \
        -o run \
        -k "thread backtrace all" \
        -k "register read" \
        -k "frame variable" \
        -k "disassemble --frame --mixed" \
        -k "process kill" \
        -- "$GAME_BIN" --no-sound --window
fi
