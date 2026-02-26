-- Example Lua Mod for Titan Engine
-- This mod demonstrates basic scripting capabilities

local ModState = {
    updateCount = 0,
    spawnedEntities = {},
    modVersion = "1.0"
}

-- Initialize the mod
function OnModInit()
    Print("=== Example Mod v" .. ModState.modVersion .. " ===")
    Print("This is a sample mod for Titan Engine")
    Print("Features:")
    Print("  - Frame counter")
    Print("  - Engine time display")
    Print("  - Delta time tracking")
end

-- Called every frame
function OnUpdate(deltaTime)
    ModState.updateCount = ModState.updateCount + 1
    
    -- Print every 60 frames (approximately every second at 60 FPS)
    if ModState.updateCount % 60 == 0 then
        local currentTime = GetTime()
        Print("[MOD] Frame: " .. ModState.updateCount .. 
              " | Time: " .. string.format("%.2f", currentTime) .. 
              " | Delta: " .. string.format("%.4f", deltaTime))
    end
end

-- Called when mod is unloaded
function OnModCleanup()
    Print("=== Example Mod Unloaded ===")
    Print("Total frames processed: " .. ModState.updateCount)
end

-- Example utility functions for future expansion

function PrintVector(label, x, y, z)
    Print(label .. ": (" .. x .. ", " .. y .. ", " .. z .. ")")
end

function CreateGameObject(name, x, y, z)
    Print("Creating game object: " .. name .. " at (" .. x .. ", " .. y .. ", " .. z .. ")")
    -- This would integrate with Entity API once fully exposed
    return {name = name, x = x, y = y, z = z}
end

function SetupLevel()
    Print("Setting up level...")
    -- Level initialization code would go here
end

Print("Example Mod Loaded!")
