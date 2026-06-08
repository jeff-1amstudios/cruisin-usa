local cpu = manager.machine.devices[":maincpu"]
local mem = cpu.spaces["program"]
local frame = 0
local sent = false

local function sendsnd(code)
  local hi = bit.rshift(code,8)
  local lo = bit.band(code,0xff)
  local function wr(v)
    mem:write_u16(0x995000, v)
    mem:write_u16(0x9a0000, v)
  end
  wr(0xff00 | hi)
  wr(0xfd00 | hi)
  wr(0xff00 | hi)
  wr(0xff00 | lo)
  wr(0xfd00 | lo)
  wr(0xff00 | lo)
end

emu.register_frame_done(function()
  frame = frame + 1
  if frame == 240 and not sent then
    -- try a known code used by tune commands (from stable.inc)
    sendsnd(0x0002)
    sent = true
  end
  if frame > 900 then
    emu.exit()
  end
end)
