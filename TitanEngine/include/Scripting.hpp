#pragma once

#include "Core.hpp"
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include <lua.hpp>

namespace Titan {

// ============================================================================
// Scripting System Interface
// ============================================================================

class ScriptingSystem : public ISystem {
public:
    virtual ~ScriptingSystem() = default;

    virtual void Initialize() override = 0;
    virtual void Update(float deltaTime) override = 0;
    virtual void Shutdown() override = 0;

    // Lua script execution
    virtual bool ExecuteScript(const std::string& scriptPath) = 0;
    virtual bool ExecuteString(const std::string& luaCode) = 0;

    // Script registration
    virtual void RegisterFunction(const std::string& name, 
                                 std::function<int(lua_State*)> func) = 0;

    // Mod loading
    virtual bool LoadScript(const std::string& scriptPath) = 0;  // Alias for ExecuteScript
    virtual bool LoadMod(const std::string& modPath) = 0;
    virtual void UnloadMod(const std::string& modName) = 0;
};

// ============================================================================
// Lua-based Scripting System Implementation
// ============================================================================

class LuaScriptingSystem : public ScriptingSystem {
private:
    lua_State* luaState{nullptr};
    std::unordered_map<std::string, std::string> loadedMods;

public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    bool ExecuteScript(const std::string& scriptPath) override;
    bool ExecuteString(const std::string& luaCode) override;

    void RegisterFunction(const std::string& name,
                         std::function<int(lua_State*)> func) override;

    bool LoadScript(const std::string& scriptPath) override;
    bool LoadMod(const std::string& modPath) override;
    void UnloadMod(const std::string& modName) override;

    lua_State* GetLuaState() const { return luaState; }

private:
    void RegisterEngineAPI();
    void RegisterEntityAPI();
    void RegisterComponentAPI();
    void RegisterInputAPI();
    void RegisterPhysicsAPI();
};

} // namespace Titan
