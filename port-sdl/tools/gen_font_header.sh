#!/usr/bin/env bash
set -euo pipefail

SRC="${1:-/Users/j.harris/code/cruisin-usa/FONT.ASM}"
OUT="${2:-/Users/j.harris/code/cruisin-usa/port-sdl/src/font1_data.h}"

perl -ne '
BEGIN {
  print "#ifndef FONT1_DATA_H\n";
  print "#define FONT1_DATA_H\n";
  print "#include <stdint.h>\n";
  print "static const uint16_t kFont1Rows[] = {\n";
}
$line = $_;
$line =~ s/;.*$//;
if ($line =~ /^\s*\.word\s+([01]+)[bB]\s*$/) {
  $bits = $1;
  $v = oct("0b" . $bits);
  printf("  0x%04X,\n", $v);
}
END {
  print "};\n";
  print "static const int kFont1RowsCount = (int)(sizeof(kFont1Rows) / sizeof(kFont1Rows[0]));\n";
  print "#endif\n";
}
' "$SRC" > "$OUT"
