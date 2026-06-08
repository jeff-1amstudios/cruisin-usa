#!/usr/bin/env python3
from __future__ import annotations

import argparse
import pathlib
import re
import struct
import sys
import zipfile
from dataclasses import dataclass
from typing import Dict, List, Optional, Sequence

ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))

from ida.walk_source_and_rom import parse_address_map, read_rom_word  # type: ignore


DEFAULT_MAP = ROOT / "tools" / "ida" / "address.map"
DEFAULT_TYPES = ROOT / "types.h"
DEFAULT_ROM = ROOT / "roms" / "crusnusa45_maindata_interleaved_bswap32.bin"
DEFAULT_REAL_ROMSET = ROOT / "roms" / "crusnusa"
ROM_WINDOW_BASE = 0x00C00000
ROM_WINDOW_WORDS = 0x00280000
PROGRAM_ROMS = {
    10: "v4.5_4-11-95_cruisn_usa_u10_86b3.u10",
    11: "v4.5_4-11-95_cruisn_usa_u11_6d73.u11",
    12: "v4.5_4-11-95_cruisn_usa_u12_4b32.u12",
    13: "v4.5_4-11-95_cruisn_usa_u13_430e.u13",
}

PRIMITIVE_SIZES = {
    "u16": 2,
    "u32": 4,
    "s32": 4,
    "f32": 4,
}

PRIMITIVE_KINDS = set(PRIMITIVE_SIZES)


@dataclass
class FieldDef:
    type_name: str
    name: str
    array_len: Optional[int]


@dataclass
class StructDef:
    name: str
    fields: List[FieldDef]


@dataclass
class OROMPolygon:
    words: Sequence[int]
    indices: Sequence[int]


@dataclass
class OROMModel:
    radius: int
    stored_polygon_count: int
    stored_vertex_count: int
    polygon_count: int
    vertex_count: int
    vertices: List[tuple[int, int, int]]
    polygons: List[OROMPolygon]


class TypesParser:
    STRUCT_RE = re.compile(
        r"typedef\s+struct\s+([A-Za-z_]\w*)\s*\{(.*?)\}\s*([A-Za-z_]\w*)\s*;",
        re.DOTALL,
    )
    FIELD_RE = re.compile(
        r"^\s*([A-Za-z_]\w*)\s+([A-Za-z_]\w*)(?:\[([A-Za-z_]\w*|\d+)\])?\s*;\s*$"
    )
    ENUM_RE = re.compile(r"enum\s*\{(.*?)\}\s*;", re.DOTALL)
    ENUM_ITEM_RE = re.compile(
        r"^\s*([A-Za-z_]\w*)\s*=\s*([A-Za-z_][A-Za-z0-9_]*|\d+)\s*$"
    )

    def __init__(self, path: pathlib.Path):
        self.path = path
        raw = path.read_text()
        self.text = self._strip_comments(raw)
        self.constants = self._parse_constants()
        self.structs = self._parse_structs()

    @staticmethod
    def _strip_comments(text: str) -> str:
        text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
        text = re.sub(r"//.*$", "", text, flags=re.MULTILINE)
        return text

    def _parse_constants(self) -> Dict[str, int]:
        out: Dict[str, int] = {}
        for m in self.ENUM_RE.finditer(self.text):
            body = m.group(1)
            for part in body.split(","):
                part = part.strip()
                if not part:
                    continue
                item = self.ENUM_ITEM_RE.match(part)
                if not item:
                    continue
                name, value_expr = item.groups()
                if value_expr.isdigit():
                    out[name] = int(value_expr, 10)
                elif value_expr in out:
                    out[name] = out[value_expr]
        return out

    def _parse_structs(self) -> Dict[str, StructDef]:
        out: Dict[str, StructDef] = {}
        for m in self.STRUCT_RE.finditer(self.text):
            tag_name, body, typedef_name = m.groups()
            if tag_name != typedef_name:
                raise ValueError(f"typedef/tag mismatch: {tag_name} vs {typedef_name}")
            fields: List[FieldDef] = []
            for raw_line in body.splitlines():
                line = raw_line.strip()
                if not line:
                    continue
                fm = self.FIELD_RE.match(line)
                if not fm:
                    raise ValueError(f"Unsupported field syntax in {typedef_name}: {raw_line!r}")
                type_name, field_name, array_len = fm.groups()
                fields.append(
                    FieldDef(
                        type_name=type_name,
                        name=field_name,
                        array_len=self._resolve_array_len(array_len) if array_len else None,
                    )
                )
            out[typedef_name] = StructDef(name=typedef_name, fields=fields)
        return out

    def _resolve_array_len(self, token: str) -> int:
        if token.isdigit():
            return int(token, 10)
        if token in self.constants:
            return self.constants[token]
        raise ValueError(f"Unknown array length token: {token}")


class StructDecoder:
    def __init__(self, structs: Dict[str, StructDef]):
        self.structs = structs
        self._size_cache: Dict[str, int] = {}

    def sizeof(self, type_name: str) -> int:
        if type_name in PRIMITIVE_SIZES:
            return PRIMITIVE_SIZES[type_name]
        if type_name in self._size_cache:
            return self._size_cache[type_name]
        struct_def = self.structs.get(type_name)
        if struct_def is None:
            raise KeyError(f"Unknown type: {type_name}")
        size = 0
        for field in struct_def.fields:
            count = field.array_len or 1
            size += self.sizeof(field.type_name) * count
        self._size_cache[type_name] = size
        return size

    def decode(self, type_name: str, data: bytes):
        if type_name in PRIMITIVE_KINDS:
            return self._decode_primitive(type_name, data)
        return self._decode_struct(type_name, data)

    def _decode_primitive(self, type_name: str, data: bytes):
        expected_size = PRIMITIVE_SIZES[type_name]
        if len(data) != expected_size:
            raise ValueError(
                f"Primitive {type_name} expected {expected_size} bytes, got {len(data)}"
            )
        raw_u32 = int.from_bytes(data, "big", signed=False)
        if type_name == "u16":
            return raw_u32
        if type_name == "u32":
            return raw_u32
        if type_name == "s32":
            return int.from_bytes(data, "big", signed=True)
        if type_name == "f32":
            return struct.unpack(">f", data)[0]
        raise KeyError(type_name)

    def _decode_struct(self, type_name: str, data: bytes):
        struct_def = self.structs[type_name]
        offset = 0
        items = []
        for field in struct_def.fields:
            field_size = self.sizeof(field.type_name)
            count = field.array_len or 1
            total_size = field_size * count
            chunk = data[offset:offset + total_size]
            if len(chunk) != total_size:
                raise ValueError(
                    f"Not enough data for {type_name}.{field.name}: need {total_size}, got {len(chunk)}"
                )
            if count == 1:
                value = self.decode(field.type_name, chunk)
            else:
                value = [
                    self.decode(field.type_name, chunk[i * field_size:(i + 1) * field_size])
                    for i in range(count)
                ]
            items.append((offset, field, value, chunk))
            offset += total_size
        return items


def lookup_label_address(map_path: pathlib.Path, label: str) -> int:
    address_map = parse_address_map(map_path)
    if label not in address_map:
        raise KeyError(f"Label {label!r} not found in {map_path}")
    return address_map[label]


def parse_address_token(token: str) -> Optional[int]:
    tok = token.strip()
    if not tok:
        return None
    if tok.lower().startswith("0x"):
        return int(tok, 16)
    if re.fullmatch(r"[0-9a-fA-F]+", tok):
        return int(tok, 16)
    return None


def map_word_address_to_rom_offset(word_addr: int) -> int:
    """
    Translate a project word address into a linear word-ROM offset.

    The game maps the program ROM at 0x00C00000..0x00E7FFFF. The canonical
    bswap32 ROM file is linear, so that ROM window becomes 0x000000..0x27FFFF.
    """
    if ROM_WINDOW_BASE <= word_addr < ROM_WINDOW_BASE + ROM_WINDOW_WORDS:
        return word_addr - ROM_WINDOW_BASE
    return word_addr


def read_words_from_rom(rom_path: pathlib.Path, word_addr: int, byte_len: int) -> bytes:
    if byte_len % 4 != 0:
        raise ValueError(f"Byte length must be a multiple of 4, got {byte_len}")
    out = bytearray()
    for i in range(byte_len // 4):
        value = read_rom_word(word_addr + i, rom_path)
        if value is None:
            raise ValueError(
                f"ROM read failed at word address 0x{word_addr + i:08X} from {rom_path}"
            )
        out.extend(value.to_bytes(4, "big"))
    return bytes(out)


def build_u10_13_image(romset_path: pathlib.Path) -> bytes:
    def read_member(name: str) -> bytes:
        if romset_path.is_dir():
            return (romset_path / name).read_bytes()
        if romset_path.is_file() and romset_path.suffix.lower() == ".zip":
            with zipfile.ZipFile(romset_path, "r") as zf:
                return zf.read(name)
        raise ValueError(f"Unsupported romset path: {romset_path}")

    parts = [read_member(PROGRAM_ROMS[u]) for u in (10, 11, 12, 13)]
    size = len(parts[0])
    if len({len(p) for p in parts}) != 1:
        raise ValueError("Program ROM sizes do not match")
    out = bytearray(size * 4)
    for i in range(size):
        out[i * 4 + 0] = parts[0][i]
        out[i * 4 + 1] = parts[1][i]
        out[i * 4 + 2] = parts[2][i]
        out[i * 4 + 3] = parts[3][i]
    return bytes(out)


def s16(v: int) -> int:
    return struct.unpack(">h", struct.pack(">H", v & 0xFFFF))[0]


def s32(v: int) -> int:
    return struct.unpack(">i", struct.pack(">I", v & 0xFFFFFFFF))[0]


def format_primitive(type_name: str, value, raw_bytes: bytes) -> str:
    raw_u32 = int.from_bytes(raw_bytes, "big", signed=False)
    if type_name == "u16":
        return f"0x{raw_u32:04X} ({raw_u32})"
    if type_name == "u32":
        return f"0x{raw_u32:08X} ({raw_u32})"
    if type_name == "s32":
        return f"{value} (0x{raw_u32:08X})"
    if type_name == "f32":
        return f"{value:.9g} (0x{raw_u32:08X})"
    raise KeyError(type_name)


def emit_view(decoder: StructDecoder, type_name: str, data: bytes, indent: int = 0) -> List[str]:
    prefix = " " * indent
    if type_name in PRIMITIVE_KINDS:
        return [prefix + format_primitive(type_name, decoder.decode(type_name, data), data)]

    lines: List[str] = []
    for offset, field, value, chunk in decoder.decode(type_name, data):
        field_prefix = " " * indent + f"+0x{offset:04X} {field.type_name} {field.name}"
        if field.array_len:
            lines.append(f"{field_prefix}[{field.array_len}]")
            element_size = decoder.sizeof(field.type_name)
            for idx in range(field.array_len):
                elem_bytes = chunk[idx * element_size:(idx + 1) * element_size]
                lines.append(" " * (indent + 2) + f"[{idx}]")
                lines.extend(emit_view(decoder, field.type_name, elem_bytes, indent + 4))
            continue

        if field.type_name in PRIMITIVE_KINDS:
            lines.append(f"{field_prefix} = {format_primitive(field.type_name, value, chunk)}")
        else:
            lines.append(field_prefix)
            lines.extend(emit_view(decoder, field.type_name, chunk, indent + 2))
    return lines


def decode_oromdata_header(blob: bytes) -> tuple[int, int, int]:
    if len(blob) < 8:
        raise ValueError("OROMDATA blob too short for header")
    radius = int.from_bytes(blob[0:4], "big", signed=False)
    stored_polygon_count = int.from_bytes(blob[4:6], "big", signed=False)
    stored_vertex_count = int.from_bytes(blob[6:8], "big", signed=False)
    return radius, stored_polygon_count, stored_vertex_count


def parse_oromdata_model(blob: bytes) -> OROMModel:
    radius, stored_polygon_count, stored_vertex_count = decode_oromdata_header(blob)
    polygon_count = stored_polygon_count + 1
    vertex_count = stored_vertex_count + 1
    need = 8 + vertex_count * 8 + polygon_count * 20
    if len(blob) < need:
        raise ValueError(
            f"OROMDATA blob too short: need {need} bytes for {vertex_count} vertices and {polygon_count} polygons, got {len(blob)}"
        )

    vertices: List[tuple[int, int, int]] = []
    vbase = 8
    for idx in range(vertex_count):
        off = vbase + idx * 8
        xy = int.from_bytes(blob[off:off + 4], "big", signed=False)
        z = s32(int.from_bytes(blob[off + 4:off + 8], "big", signed=False))
        x = s16(xy & 0xFFFF)
        y = s16((xy >> 16) & 0xFFFF)
        vertices.append((x, y, z))

    polygons: List[OROMPolygon] = []
    pbase = vbase + vertex_count * 8
    for idx in range(polygon_count):
        off = pbase + idx * 20
        words = [
            int.from_bytes(blob[off + i * 4:off + (i + 1) * 4], "big", signed=False)
            for i in range(5)
        ]
        idx_word = words[1]
        indices = [
            idx_word & 0xFF,
            (idx_word >> 8) & 0xFF,
            (idx_word >> 16) & 0xFF,
            (idx_word >> 24) & 0xFF,
        ]
        polygons.append(OROMPolygon(words=words, indices=indices))

    return OROMModel(
        radius=radius,
        stored_polygon_count=stored_polygon_count,
        stored_vertex_count=stored_vertex_count,
        polygon_count=polygon_count,
        vertex_count=vertex_count,
        vertices=vertices,
        polygons=polygons,
    )


def parse_oromdata_from_u10_13(image: bytes, word_addr: int) -> OROMModel:
    rom_word_addr = map_word_address_to_rom_offset(word_addr)
    off = rom_word_addr * 4
    header = image[off:off + 8]
    if len(header) < 8:
        raise ValueError(f"ROM set image too short for OROMDATA header at 0x{word_addr:08X}")
    radius = struct.unpack_from("<I", header, 0)[0]
    header_word = struct.unpack_from("<I", header, 4)[0]
    vertex_count = (header_word & 0xFF) + 1
    polygon_count = ((header_word >> 16) & 0xFFFF) + 1
    size = 8 + vertex_count * 8 + polygon_count * 20
    blob = image[off:off + size]
    if len(blob) < size:
        raise ValueError(f"ROM set image too short for full OROMDATA at 0x{word_addr:08X}")
    vertices: List[tuple[int, int, int]] = []
    vbase = 8
    for idx in range(vertex_count):
        xy, z = struct.unpack_from("<Ii", blob, vbase + idx * 8)
        x = s16(xy & 0xFFFF)
        y = s16((xy >> 16) & 0xFFFF)
        vertices.append((x, y, z))

    polygons: List[OROMPolygon] = []
    pbase = vbase + vertex_count * 8
    for idx in range(polygon_count):
        words = list(struct.unpack_from("<IIIII", blob, pbase + idx * 20))
        idx_word = words[1]
        indices = [
            idx_word & 0xFF,
            (idx_word >> 8) & 0xFF,
            (idx_word >> 16) & 0xFF,
            (idx_word >> 24) & 0xFF,
        ]
        polygons.append(OROMPolygon(words=words, indices=indices))

    return OROMModel(
        radius=radius,
        stored_polygon_count=polygon_count - 1,
        stored_vertex_count=vertex_count - 1,
        polygon_count=polygon_count,
        vertex_count=vertex_count,
        vertices=vertices,
        polygons=polygons,
    )


def emit_oromdata_polygons(decoder: StructDecoder, blob: bytes, limit: int) -> List[str]:
    model = parse_oromdata_model(blob)
    vertex_size = decoder.sizeof("ROM_VERTEX")
    polygon_size = decoder.sizeof("ROM_POLYGON")
    poly_base = decoder.sizeof("OROMDATA") + model.vertex_count * vertex_size
    vertex_base = decoder.sizeof("OROMDATA")
    max_count = min(limit, model.polygon_count)

    lines = [
        "",
        (
            "OROMDATA summary: "
            f"stored_polygons={model.stored_polygon_count} actual_polygons={model.polygon_count}, "
            f"stored_vertices={model.stored_vertex_count} actual_vertices={model.vertex_count}"
        ),
        f"polygon array @ +0x{poly_base:04X}",
    ]
    for idx in range(max_count):
        off = poly_base + idx * polygon_size
        chunk = blob[off:off + polygon_size]
        if len(chunk) != polygon_size:
            lines.append(f"[{idx}] truncated at +0x{off:04X}")
            break
        poly = model.polygons[idx]
        v1, v2, v3, v4 = poly.indices
        validity = "ok" if max(v1, v2, v3, v4) < model.vertex_count else "out-of-range"
        lines.append(f"[{idx}] +0x{off:04X}")
        lines.extend(emit_view(decoder, "ROM_POLYGON", chunk, indent=2))
        lines.append(
            f"  vertices: v1={v1} v2={v2} v3={v3} v4={v4} ({validity}, vertex_count={model.vertex_count})"
        )
        for name, vidx in (("v1", v1), ("v2", v2), ("v3", v3), ("v4", v4)):
            if vidx >= model.vertex_count:
                lines.append(f"    {name}: out-of-range")
                continue
            voff = vertex_base + vidx * vertex_size
            vchunk = blob[voff:voff + vertex_size]
            vx_y = int.from_bytes(vchunk[0:4], "big", signed=False)
            vz = s32(int.from_bytes(vchunk[4:8], "big", signed=False))
            vx = s16(vx_y & 0xFFFF)
            vy = s16((vx_y >> 16) & 0xFFFF)
            lines.append(
                f"    {name}: idx={vidx} @ +0x{voff:04X} x={vx} y={vy} z={vz} raw=0x{vx_y:08X}"
            )
    if max_count < model.polygon_count:
        lines.append(f"... {model.polygon_count - max_count} more polygons not shown")
    return lines


def validate_oromdata_against_romset(blob: bytes, word_addr: int, romset_path: pathlib.Path) -> List[str]:
    ours = parse_oromdata_model(blob)
    other = parse_oromdata_from_u10_13(build_u10_13_image(romset_path), word_addr)
    lines = ["", f"OROMDATA validation against raw ROM set: {romset_path}"]

    if (ours.radius, ours.vertex_count, ours.polygon_count) != (
        other.radius,
        other.vertex_count,
        other.polygon_count,
    ):
        lines.append(
            "  header mismatch: "
            f"ours radius={ours.radius} verts={ours.vertex_count} polys={ours.polygon_count}, "
            f"raw radius={other.radius} verts={other.vertex_count} polys={other.polygon_count}"
        )
        return lines

    if ours.vertices != other.vertices:
        mismatch = next(
            idx for idx, (a, b) in enumerate(zip(ours.vertices, other.vertices)) if a != b
        )
        lines.append(
            f"  vertex mismatch at {mismatch}: ours={ours.vertices[mismatch]} raw={other.vertices[mismatch]}"
        )
        return lines

    for idx, (a, b) in enumerate(zip(ours.polygons, other.polygons)):
        if list(a.words) != list(b.words) or list(a.indices) != list(b.indices):
            lines.append(
                f"  polygon mismatch at {idx}: ours_words={list(a.words)} raw_words={list(b.words)} "
                f"ours_idx={list(a.indices)} raw_idx={list(b.indices)}"
            )
            return lines

    lines.append(
        f"  match: radius={ours.radius}, vertices={ours.vertex_count}, polygons={ours.polygon_count}"
    )
    lines.append(f"  first vertex: {ours.vertices[0]}")
    lines.append(f"  first polygon indices: {list(ours.polygons[0].indices)}")
    return lines


def build_parser() -> argparse.ArgumentParser:
    ap = argparse.ArgumentParser(
        description="Decode a ROM-backed label as a struct from types.h."
    )
    ap.add_argument(
        "label_or_address",
        help="Exact label name from tools/ida/address.map, or a raw hex word address",
    )
    ap.add_argument("struct_name", help="Struct typedef name from types.h")
    ap.add_argument("--map", type=pathlib.Path, default=DEFAULT_MAP)
    ap.add_argument("--types", type=pathlib.Path, default=DEFAULT_TYPES)
    ap.add_argument("--rom", type=pathlib.Path, default=DEFAULT_ROM)
    ap.add_argument(
        "--bytes",
        type=int,
        default=None,
        help="Override byte count to read; defaults to sizeof(struct_name)",
    )
    ap.add_argument(
        "--oromdata-polygons",
        type=int,
        default=0,
        help="When struct_name is OROMDATA, also decode the first N ROM_POLYGON entries",
    )
    ap.add_argument(
        "--validate-romset",
        type=pathlib.Path,
        default=None,
        help="For OROMDATA, validate decoded vertices/polygons against raw u10-u13 ROMs from a directory or zip",
    )
    return ap


def main() -> int:
    args = build_parser().parse_args()

    parser = TypesParser(args.types)
    if args.struct_name not in parser.structs:
        known = ", ".join(sorted(parser.structs))
        raise SystemExit(f"Unknown struct {args.struct_name!r}. Known structs: {known}")

    decoder = StructDecoder(parser.structs)
    size = args.bytes if args.bytes is not None else decoder.sizeof(args.struct_name)
    if size <= 0:
        raise SystemExit(f"Invalid read size {size}")

    raw_addr = parse_address_token(args.label_or_address)
    if raw_addr is None:
        label = args.label_or_address
        word_addr = lookup_label_address(args.map, label)
    else:
        label = None
        word_addr = raw_addr

    rom_word_addr = map_word_address_to_rom_offset(word_addr)
    if args.oromdata_polygons:
        if args.struct_name != "OROMDATA":
            raise SystemExit("--oromdata-polygons requires struct_name OROMDATA")
        header_blob = read_words_from_rom(args.rom, rom_word_addr, decoder.sizeof("OROMDATA"))
        _, stored_polygon_count, stored_vertex_count = decode_oromdata_header(
            header_blob
        )
        polygon_count = stored_polygon_count + 1
        vertex_count = stored_vertex_count + 1
        needed_size = (
            decoder.sizeof("OROMDATA")
            + vertex_count * decoder.sizeof("ROM_VERTEX")
            + polygon_count * decoder.sizeof("ROM_POLYGON")
        )
        if args.bytes is None:
            size = needed_size
    byte_off = rom_word_addr * 4
    blob = read_words_from_rom(args.rom, rom_word_addr, size)

    print(f"label      : {label if label is not None else '(direct address)'}")
    print(f"struct     : {args.struct_name}")
    print(f"word_addr  : 0x{word_addr:08X}")
    print(f"rom_word   : 0x{rom_word_addr:08X}")
    print(f"byte_off   : 0x{byte_off:08X}")
    print(f"size       : {size} bytes")
    print(f"rom        : {args.rom}")
    print()
    for line in emit_view(decoder, args.struct_name, blob):
        print(line)
    if args.oromdata_polygons:
        for line in emit_oromdata_polygons(decoder, blob, args.oromdata_polygons):
            print(line)
    if args.validate_romset is not None:
        if args.struct_name != "OROMDATA":
            raise SystemExit("--validate-romset requires struct_name OROMDATA")
        for line in validate_oromdata_against_romset(blob, word_addr, args.validate_romset):
            print(line)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
