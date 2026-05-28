-- Cruis'n USA debugger helper: breakpoint + logerror only.
--
-- Usage:
--   mame crusnusa -debug -autoboot_script /Users/j.harris/code/cruisin-usa/tools/mame/crusnusa_trace_sections.lua

local machine = manager.machine
local cpu = machine.devices[":maincpu"]
if not machine.debugger or not cpu or not cpu.debug then
  error("Need -debug and :maincpu debugger support.")
end

local LOAD_SECTION_REQ = 0xA3ED
local action = [[logerror "LOAD_SECTION_REQ pc=%06X ar2=%06X src=%08X dst=%08X s0=%08X s1=%08X\n",pc,ar2,d@ar2,d@(ar2+1),d@(d@ar2),d@(d@ar2+1); g]]
cpu.debug:bpset(LOAD_SECTION_REQ, nil, action)

local function initialize_my_debugger()
    local cpu = manager.machine.devices[":maincpu"]

    -- Check if the debugger subsystem is fully initialized
    if manager.machine.debug then
        local dev_debug = cpu.debug
        local global_symbols = dev_debug.symbols

        if global_symbols then
            global_symbols:add("hi", 4000)
            print("Success: Symbol 'hi' added to global CPU symbols!")

            -- Set up your breakpoint safely now
            dev_debug:bpset(0x1234, "A == hi")
            print("Breakpoint successfully armed!")
        else
            print("Error: symbols table could not be found.")
        end
    else
        print("Error: Debugger skipped or failed to initialize.")
    end
end

emu.add_on_load(function()
    print("Machine loaded! Initializing custom symbols...")
    initialize_my_debugger()
end)
