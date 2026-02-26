// Minimal Lua stub for linking tests without real Lua
extern "C" {
    int lua_gettop(void* L) { return 0; }
    void lua_pop(void* L, int n) { (void)L; (void)n; }
    void lua_close(void* L) { (void)L; }
}
