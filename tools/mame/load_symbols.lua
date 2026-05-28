-- Load debugger symbols from a plain text file.
--
-- Usage:
--   mame crusnusa -debug -autoboot_script /Users/j.harris/code/cruisin-usa/tools/mame/load_symbols.lua
--
-- Symbol file format (one per line):
--   <hex_addr> <name>
-- Example:
--   A3ED LOAD_SECTION_REQ
--   A2F9 DECOMPRESS
--   4AE6 C_INT00

local dbg = manager.machine.debugger
if not dbg then
  error("Debugger not available. Start MAME with -debug.")
end

local sym_path = "/Users/j.harris/code/cruisin-usa/tools/mame/game.sym"

local function add_symbol(name, addr_hex)
  dbg:command(string.format("symadd %s,0x%s", name, addr_hex))
end

local loaded = 0
local skipped = 0

local f = io.open(sym_path, "r")
if not f then
  error("Could not open symbol file: " .. sym_path)
end

for line in f:lines() do
  local trimmed = line:gsub("^%s+", ""):gsub("%s+$", "")
  if trimmed == "" or trimmed:sub(1, 1) == "#" or trimmed:sub(1, 2) == "//" then
    skipped = skipped + 1
  else
    local addr, name = trimmed:match("^([%x]+)%s+([%w_%.%-%$@]+)$")
    if addr and name then
      add_symbol(name, addr)
      loaded = loaded + 1
    else
      skipped = skipped + 1
      print("[sym] skipped malformed line: " .. line)
    end
  end
end
f:close()

print(string.format("[sym] loaded %d symbols from %s (%d skipped)", loaded, sym_path, skipped))
