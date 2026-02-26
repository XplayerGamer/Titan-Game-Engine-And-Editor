// Minimal stub for lua.hpp to allow project to compile without the Lua SDK.
// Replace with the real Lua headers or link against Lua for scripting support.
#pragma once

extern "C" {
    typedef struct lua_State lua_State;
    typedef long long lua_Integer;
}
// Minimal stubs for functions used by the scripting system. These are
// placeholders — replace with the real Lua SDK for full scripting support.
inline lua_State* luaL_newstate() { return nullptr; }
inline void lua_close(lua_State*) {}
inline void luaL_openlibs(lua_State*) {}
inline int luaL_dostring(lua_State*, const char*) { return 0; }
inline void lua_getglobal(lua_State*, const char*) {}
inline int lua_isfunction(lua_State*, int) { return 0; }
inline void lua_pushnumber(lua_State*, double) {}
inline void lua_pushboolean(lua_State*, int) {}
inline void lua_pushinteger(lua_State*, lua_Integer) {}
inline int lua_pcall(lua_State*, int, int, int) { return 0; }
inline const char* lua_tostring(lua_State*, int) { return ""; }
inline void lua_pop(lua_State*, int) {}
inline void lua_register(lua_State*, const char*, int(*)(lua_State*)) {}
inline int lua_gettop(lua_State*) { return 0; }
inline int lua_isstring(lua_State*, int) { return 0; }
inline int lua_isnumber(lua_State*, int) { return 0; }
inline int lua_isinteger(lua_State*, int) { return 0; }
inline double lua_tonumber(lua_State*, int) { return 0.0; }
inline lua_Integer lua_tointeger(lua_State*, int) { return 0; }

#ifndef LUA_OK
#define LUA_OK 0
#endif
