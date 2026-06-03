import idc
import shutil

try:
    import ida_loader
except Exception:
    ida_loader = None

start_ea = 0x00000000
end_ea_excl = 0x00271ACC + 1  # exclusive end

lst_c = r"C:\cruisin\crusnusa45_maindata_interleaved_bswap32.bin.lst"
lst_z = r"Z:\cruisin\crusnusa45_maindata_interleaved_bswap32.bin.lst"
map_c = r"C:\cruisin\address.map"
map_z = r"Z:\cruisin\address.map"


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


ofile_lst = get_const("OFILE_LST")
ofile_map = get_const("OFILE_MAP", 0)
badaddr = get_const("BADADDR", 0xFFFFFFFF)

print(f"Starting LST generation: {lst_c} (0x{start_ea:08X}-0x{end_ea_excl-1:08X})")
ok = gen_file(ofile_lst, lst_c, start_ea, end_ea_excl, 0)
if not ok:
    raise RuntimeError("LST generation failed")

print(f"Copying LST to Z: {lst_z}")
shutil.copy2(lst_c, lst_z)

print(f"Starting MAP generation: {map_c}")
ok = gen_file(ofile_map, map_c, 0, badaddr, 0)
if not ok:
    raise RuntimeError("MAP generation failed")

print(f"Copying MAP to Z: {map_z}")
shutil.copy2(map_c, map_z)

print("Finished.")
print("Wrote LST:", lst_c)
print("Copied LST:", lst_z)
print("Wrote MAP:", map_c)
print("Copied MAP:", map_z)
