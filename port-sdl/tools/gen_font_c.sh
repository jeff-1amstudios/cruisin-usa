#!/usr/bin/env bash
set -euo pipefail

SRC="${1:-/Users/j.harris/code/cruisin-usa/FONT.ASM}"
OUT="${2:-/Users/j.harris/code/cruisin-usa/port-sdl/src/FONT.c}"

{
  echo '#include "FONT.h"'
  echo
  echo 'const uint16_t kFont1Rows[] = {'
  perl -ne '
    $line = $_;
    $line =~ s/;.*$//;
    if ($line =~ /^\s*\.word\s+([01]+)[bB]\s*$/) {
      $bits = $1;
      $v = oct("0b" . $bits);
      printf("    0x%04X,\n", $v);
    }
  ' "$SRC"
  echo '};'
  echo 'const int kFont1RowsCount = (int)(sizeof(kFont1Rows) / sizeof(kFont1Rows[0]));'
} > "$OUT"
