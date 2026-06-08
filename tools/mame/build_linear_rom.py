#!/usr/bin/env python3
"""Rebuild a linear Cruis'n USA ROM region from MAME split ROMs.

By default, this reconstructs the full `maindata` region for a machine
(e.g. crusnusa41) into one flat binary using offsets from `mame -listxml`.
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
import zipfile
from pathlib import Path

U_RE = re.compile(r"\.u(\d+)$", re.IGNORECASE)


def parse_machine_xml(machine: str) -> ET.Element:
    proc = subprocess.run(
        ["mame", "-listxml", machine],
        check=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    root = ET.fromstring(proc.stdout)
    m = root.find(f"./machine[@name='{machine}']")
    if m is None:
        raise ValueError(f"Machine '{machine}' not found in mame -listxml output")
    return m


def parse_offset(text: str) -> int:
    return int(text, 16)


def u_from_rom_name(name: str) -> int:
    m = U_RE.search(name)
    if not m:
        raise ValueError(f"ROM name does not end with .uNN: {name}")
    return int(m.group(1))


def find_u_file(names: list[str], u_num: int) -> str:
    matches = [n for n in names if U_RE.search(n) and int(U_RE.search(n).group(1)) == u_num]
    if not matches:
        raise FileNotFoundError(f"Could not find file ending in .u{u_num} in zip")
    return sorted(matches)[0]


def rebuild(zip_path: Path, machine: str, region: str, out_path: Path) -> None:
    machine_xml = parse_machine_xml(machine)
    rom_nodes = [r for r in machine_xml.findall("rom") if r.get("region") == region]
    if not rom_nodes:
        raise ValueError(f"No rom entries for region '{region}' in machine '{machine}'")

    layout: list[tuple[int, int]] = []  # (u_num, offset)
    for r in rom_nodes:
        rom_name = r.get("name")
        offset_s = r.get("offset")
        if not rom_name or offset_s is None:
            continue
        u_num = u_from_rom_name(rom_name)
        offset = parse_offset(offset_s)
        layout.append((u_num, offset))

    with zipfile.ZipFile(zip_path, "r") as zf:
        names = zf.namelist()
        u_to_zip_name: dict[int, str] = {}
        u_to_data: dict[int, bytes] = {}

        for u_num, _ in layout:
            if u_num in u_to_data:
                continue
            zname = find_u_file(names, u_num)
            u_to_zip_name[u_num] = zname
            u_to_data[u_num] = zf.read(zname)

    out_size = 0
    for u_num, offset in layout:
        data = u_to_data[u_num]
        base = offset & ~0x3
        lane = offset & 0x3
        end = base + ((len(data) - 1) * 4) + lane + 1
        if end > out_size:
            out_size = end

    out = bytearray(out_size)
    for u_num, offset in layout:
        data = u_to_data[u_num]
        base = offset & ~0x3
        lane = offset & 0x3
        for i, b in enumerate(data):
            out[base + i * 4 + lane] = b

    out_path.write_bytes(out)

    print(f"Machine    : {machine}")
    print(f"Region     : {region}")
    print(f"Input zip  : {zip_path}")
    print(f"Output     : {out_path}")
    print(f"Size       : {len(out)} bytes (0x{len(out):X})")
    print("Loaded ROMs:")
    for u_num, offset in sorted(layout, key=lambda x: x[1]):
        print(f"  {u_to_zip_name[u_num]:60s} -> 0x{offset:06X}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("zip", type=Path, help="Path to crusnusa*.zip ROM set")
    ap.add_argument("--machine", default="crusnusa41", help="MAME machine shortname")
    ap.add_argument("--region", default="maindata", help="MAME ROM region")
    ap.add_argument(
        "-o",
        "--out",
        type=Path,
        default=Path("roms/crusnusa41_maindata_linear.bin"),
        help="Output binary path",
    )
    args = ap.parse_args()

    if not args.zip.exists():
        print(f"Zip not found: {args.zip}", file=sys.stderr)
        return 1

    try:
        rebuild(args.zip, args.machine, args.region, args.out)
    except Exception as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 2

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
