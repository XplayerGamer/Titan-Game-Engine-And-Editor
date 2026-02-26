#include "../include/Scripting.hpp"
#include "../include/Engine.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace Titan {

// ============================================================================
// LuaScriptingSystem Implementation
// ============================================================================

void LuaScriptingSystem::Initialize() {
    std::cout << "Lua Scripting system initializing..." << std::endl;

    luaState = luaL_newstate();
    if (!luaState) {
        std::cerr << "Warning: Failed to create Lua state! Continuing without scripting." << std::endl;
        return;
    }

    // Load standard libraries
    luaL_openlibs(luaState);

    // Register engine API
    RegisterEngineAPI();
    RegisterEntityAPI();
    RegisterComponentAPI();
    RegisterInputAPI();
    RegisterPhysicsAPI();

    std::cout << "Lua Scripting system initialized" << std::endl;
}

void LuaScriptingSystem::Update(float deltaTime) {
    // Call Lua update function if it exists
    lua_getglobal(luaState, "OnUpdate");
    if (lua_isfunction(luaState, -1)) {
        lua_pushnumber(luaState, deltaTime);
        if (lua_pcall(luaState, 1, 0, 0) != LUA_OK) {
            std::cerr << "Lua error in OnUpdate: " << lua_tostring(luaState, -1) << std::endl;
            lua_pop(luaState, 1);
        }
    } else {
        lua_pop(luaState, 1);
    }
}

void LuaScriptingSystem::Shutdown() {
    std::cout << "Lua Scripting system shutdown" << std::endl;
    
    if (luaState) {
        lua_close(luaState);
        luaState = nullptr;
    }

    loadedMods.clear();
}

bool LuaScriptingSystem::ExecuteScript(const std::string& scriptPath) {
    std::ifstream file(scriptPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open script: " << scriptPath << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return ExecuteString(buffer.str());
}

bool LuaScriptingSystem::LoadScript(const std::string& scriptPath) {
    return ExecuteScript(scriptPath);
}

bool LuaScriptingSystem::ExecuteString(const std::string& luaCode) {
    if (luaL_dostring(luaState, luaCode.c_str()) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(luaState, -1) << std::endl;
        lua_pop(luaState, 1);
        return false;
    }
    return true;
}

void LuaScriptingSystem::RegisterFunction(const std::string& name,
                                          std::function<int(lua_State*)> func) {
    // This would require storing function pointers in a way that Lua can call them
    // For now, this is a placeholder
    std::cout << "Registered Lua function: " << name << std::endl;
}

bool LuaScriptingSystem::LoadMod(const std::string& modPath) {
    std::cout << "Loading mod: " << modPath << std::endl;
    
    if (!ExecuteScript(modPath)) {
        std::cerr << "Failed to load mod: " << modPath << std::endl;
        return false;
    }

    loadedMods[modPath] = modPath;
    return true;
}

void LuaScriptingSystem::UnloadMod(const std::string& modName) {
    std::cout << "Unloading mod: " << modName << std::endl;
    loadedMods.erase(modName);

    // Call mod cleanup function if it exists
    std::string cleanupFunc = modName + "_Cleanup";
    lua_getglobal(luaState, cleanupFunc.c_str());
    if (lua_isfunction(luaState, -1)) {
        lua_pcall(luaState, 0, 0, 0);
    }
    lua_pop(luaState, 1);
}

void LuaScriptingSystem::RegisterEngineAPI() {
    // Engine functions
    lua_register(luaState, "GetTime", [](lua_State* L) -> int {
        auto& engine = GetEngine();
        lua_pushnumber(L, engine.GetElapsedTime());
        return 1;
    });

    lua_register(luaState, "GetDeltaTime", [](lua_State* L) -> int {
        auto& engine = GetEngine();
        lua_pushnumber(L, engine.GetDeltaTime());
        return 1;
    });

    lua_register(luaState, "Print", [](lua_State* L) -> int {
        int argc = lua_gettop(L);
        for (int i = 1; i <= argc; ++i) {
            if (lua_isstring(L, i)) {
                std::cout << lua_tostring(L, i);
            } else if (lua_isnumber(L, i)) {
                std::cout << lua_tonumber(L, i);
            }
        }
        std::cout << std::endl;
        return 0;
    });
}

void LuaScriptingSystem::RegisterEntityAPI() {
    // Entity management functions
    lua_register(luaState, "CreateEntity", [](lua_State* L) -> int {
        auto& engine = GetEngine();
        Titan::EntityID entityId = engine.GetEntityManager().CreateEntity();
        lua_pushinteger(L, static_cast<lua_Integer>(entityId));
        return 1;
    });

    lua_register(luaState, "DestroyEntity", [](lua_State* L) -> int {
        if (lua_gettop(L) < 1 || !lua_isinteger(L, 1)) {
            lua_pushboolean(L, false);
            return 1;
        }
        auto& engine = GetEngine();
        Titan::EntityID entityId = static_cast<Titan::EntityID>(lua_tointeger(L, 1));
        engine.GetEntityManager().DestroyEntity(entityId);
        lua_pushboolean(L, true);
        return 1;
    });

    lua_register(luaState, "GetEntityCount", [](lua_State* L) -> int {
        auto& engine = GetEngine();
        size_t count = engine.GetEntityManager().GetAllEntities().size();
        lua_pushinteger(L, static_cast<lua_Integer>(count));
        return 1;
    });
}

void LuaScriptingSystem::RegisterComponentAPI() {
    // Component management functions
    lua_register(luaState, "AddComponent", [](lua_State* L) -> int {
        // Placeholder - would need component type and entity ID
        lua_pushboolean(L, false);
        return 1;
    });

    lua_register(luaState, "RemoveComponent", [](lua_State* L) -> int {
        // Placeholder - would need component type and entity ID
        lua_pushboolean(L, false);
        return 1;
    });

    lua_register(luaState, "HasComponent", [](lua_State* L) -> int {
        // Placeholder - would need component type and entity ID
        lua_pushboolean(L, false);
        return 1;
    });
}

void LuaScriptingSystem::RegisterInputAPI() {
    // Input functions
    lua_register(luaState, "IsKeyPressed", [](lua_State* L) -> int {
        if (lua_gettop(L) < 1 || !lua_isinteger(L, 1)) {
            lua_pushboolean(L, false);
            return 1;
        }
        auto& engine = GetEngine();
        int keyCode = static_cast<int>(lua_tointeger(L, 1));
        bool pressed = engine.GetInputSystem().IsKeyPressed(static_cast<Titan::KeyCode>(keyCode));
        lua_pushboolean(L, pressed);
        return 1;
    });

    lua_register(luaState, "IsMouseButtonPressed", [](lua_State* L) -> int {
        if (lua_gettop(L) < 1 || !lua_isinteger(L, 1)) {
            lua_pushboolean(L, false);
            return 1;
        }
        auto& engine = GetEngine();
        int button = static_cast<int>(lua_tointeger(L, 1));
        bool pressed = engine.GetInputSystem().IsMouseButtonPressed(static_cast<Titan::MouseButton>(button));
        lua_pushboolean(L, pressed);
        return 1;
    });

    lua_register(luaState, "GetMousePosition", [](lua_State* L) -> int {
        auto& engine = GetEngine();
        float x, y;
        engine.GetInputSystem().GetMousePosition(x, y);
        lua_pushnumber(L, x);
        lua_pushnumber(L, y);
        return 2;
    });
}

void LuaScriptingSystem::RegisterPhysicsAPI() {
    // Physics functions
    lua_register(luaState, "CreatePhysicsBody", [](lua_State* L) -> int {
        // Placeholder - would create a physics body
        lua_pushinteger(L, 0); // Return body ID
        return 1;
    });

    lua_register(luaState, "DestroyPhysicsBody", [](lua_State* L) -> int {
        if (lua_gettop(L) < 1 || !lua_isinteger(L, 1)) {
            lua_pushboolean(L, false);
            return 1;
        }
        // Placeholder - would destroy physics body
        lua_pushboolean(L, true);
        return 1;
    });

    lua_register(luaState, "ApplyForce", [](lua_State* L) -> int {
        // Placeholder - would apply force to physics body
        lua_pushboolean(L, true);
        return 1;
    });

    lua_register(luaState, "SetGravity", [](lua_State* L) -> int {
        if (lua_gettop(L) < 3 || !lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3)) {
            lua_pushboolean(L, false);
            return 1;
        }
        auto& engine = GetEngine();
        float x = static_cast<float>(lua_tonumber(L, 1));
        float y = static_cast<float>(lua_tonumber(L, 2));
        float z = static_cast<float>(lua_tonumber(L, 3));
        // engine.GetPhysicsSystem().SetGravity({x, y, z});
        lua_pushboolean(L, true);
        return 1;
    });
}

} // namespace Titan
