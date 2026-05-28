#!/usr/bin/env python3
"""Apply Cruis'n USA (TMS320C31) vector labels in IDA.

Usage in IDA:
  File -> Script file... -> select this script

Assumes the ROM was loaded at base address 0x000000.
"""

import ida_bytes
import ida_funcs
import ida_kernwin
import ida_name
import ida_segment

BASE = 0x000000
VECTOR_COUNT = 0x40  # 64 vector entries
ENTRY_SIZE = 4       # 32-bit addresses

# Known names from CUSA.ASM vector section comments.
KNOWN = {
    0x00: "vec_reset__c_int00",
    0x01: "vec_int0_display",
    0x02: "vec_int1_tv30",
    0x03: "vec_int2_comm",
    0x04: "vec_int3_atod",
    0x05: "vec_xint0",
    0x06: "vec_rint0",
    0x07: "vec_xint1",
    0x08: "vec_rint1",
    0x09: "vec_tint0",
    0x0A: "vec_tint1",
    0x0B: "vec_dint",
}


def in_mapped_memory(ea: int) -> bool:
    return ida_segment.getseg(ea) is not None


def ensure_dword(ea: int) -> int:
    ida_bytes.del_items(ea, ida_bytes.DELIT_SIMPLE, ENTRY_SIZE)
    ida_bytes.create_dword(ea, ENTRY_SIZE)
    return ida_bytes.get_dword(ea) & 0xFFFFFFFF


def set_name_safe(ea: int, name: str) -> None:
    ida_name.set_name(ea, name, ida_name.SN_FORCE)


def maybe_make_func(ea: int) -> bool:
    if not in_mapped_memory(ea):
        return False
    if ida_funcs.get_func(ea):
        return True
    return bool(ida_funcs.add_func(ea))


def main() -> None:
    created = 0
    named = 0

    for idx in range(VECTOR_COUNT):
        vec_ea = BASE + idx * ENTRY_SIZE
        target = ensure_dword(vec_ea)

        vec_name = KNOWN.get(idx, f"vec_{idx:02X}")
        set_name_safe(vec_ea, vec_name)
        named += 1

        ptr_name = f"{vec_name}_target_{target:08X}"
        if in_mapped_memory(target):
            set_name_safe(target, ptr_name)
            if maybe_make_func(target):
                created += 1

    reset_target = ida_bytes.get_dword(BASE) & 0xFFFFFFFF
    maybe_make_func(reset_target)

    ida_kernwin.msg(
        "[crusnusa] Applied %d vectors, attempted function creation at vector targets, reset=0x%08X\n"
        % (named, reset_target)
    )


if __name__ == "__main__":
    main()
