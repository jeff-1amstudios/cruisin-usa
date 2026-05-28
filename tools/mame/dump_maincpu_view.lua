-- Dump the live maincpu program-space view as seen by MAME.
--
-- Usage:
--   mame crusnusa -debug -autoboot_script /Users/j.harris/code/cruisin-usa/tools/mame/dump_maincpu_view.lua
--
-- Output:
--   /Users/j.harris/code/cruisin-usa/roms/crusnusa41_maincpu_view.bin
--   /Users/j.harris/code/cruisin-usa/roms/crusnusa41_vectors_at_0.txt
--
-- Notes:
-- - This dumps 32-bit words from 0xC00000..0xE7FFFF (maindata mapping).
-- - It also records the first vector words from address 0x000000 to show the
--   runtime reset-vector target actually seen by the CPU.

local cpu = manager.machine.devices[":maincpu"]
assert(cpu, "missing :maincpu")
local mem = cpu.spaces["program"]
assert(mem, "missing program space")

local out_bin = "/Users/j.harris/code/cruisin-usa/roms/crusnusa41_maincpu_view.bin"
local out_vec = "/Users/j.harris/code/cruisin-usa/roms/crusnusa41_vectors_at_0.txt"

local start_addr = 0xC00000
local end_addr   = 0xE7FFFF
local words = end_addr - start_addr + 1

local f = assert(io.open(out_bin, "wb"))
for a = start_addr, end_addr do
  local w = mem:read_u32(a)
  -- Emit big-endian bytes so word value display matches d@ reads.
  local b0 = (w >> 24) & 0xFF
  local b1 = (w >> 16) & 0xFF
  local b2 = (w >> 8) & 0xFF
  local b3 = w & 0xFF
  f:write(string.char(b0, b1, b2, b3))
end
f:close()

local v = assert(io.open(out_vec, "w"))
for a = 0, 0x20 do
  v:write(string.format("%06X: %08X\n", a, mem:read_u32(a)))
end
v:close()

print(string.format("[dump] wrote %d words (%d bytes) to %s", words, words * 4, out_bin))
print(string.format("[dump] wrote runtime vectors to %s", out_vec))
