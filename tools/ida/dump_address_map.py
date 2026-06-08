import idc
import shutil

try:
    import ida_loader
except Exception:
    ida_loader = None

map_c = r"C:\cruisin\ida-address.map"
map_z = r"Z:\cruisin\ida-address.map"


def get_const(name, default=None):
    if hasattr(idc, name):
        return getattr(idc, name)
    if ida_loader is not None and hasattr(ida_loader, name):
        return getattr(ida_loader, name)
    return default


def gen_file(file_type, out_path, ea1, ea2, flags=0):
    # IDA 7.6 usually has idc.gen_file; some builds expose idc.generate_file.
    if hasattr(idc, "gen_file"):
        return idc.gen_file(file_type, out_path, ea1, ea2, flags)
    return idc.generate_file(file_type, out_path, ea1, ea2, flags)


ofile_map = get_const("OFILE_MAP", 0)
badaddr = get_const("BADADDR", 0xFFFFFFFF)

print(f"Starting MAP generation: {map_c}")
ok = gen_file(ofile_map, map_c, 0, badaddr, 0)
if not ok:
    raise RuntimeError("MAP generation failed")

print(f"Copying MAP to Z: {map_z}")
shutil.copy2(map_c, map_z)

print("Finished.")
print("Wrote MAP:", map_c)
print("Copied MAP:", map_z)
