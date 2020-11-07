#include "api.h"
#include "core/os.h"
#include "core/util.h"
#include "lib/lua-cjson/lua_cjson.h"
#include "lib/lua-enet/enet.h"
#include "lib/lua-xxhash-bind-h/xxhash_bind.h"
#include "lib/lua-bitop/lua-bitop.h"
#include "lib/lua-compat-5.3-utf8/lutf8lib.h"

const luaL_Reg lovrModules[] = {
  { "lovr", luaopen_lovr },
#ifdef LOVR_ENABLE_AUDIO
  { "lovr.audio", luaopen_lovr_audio },
#endif
#ifdef LOVR_ENABLE_DATA
  { "lovr.data", luaopen_lovr_data },
#endif
#ifdef LOVR_ENABLE_EVENT
  { "lovr.event", luaopen_lovr_event },
#endif
#ifdef LOVR_ENABLE_FILESYSTEM
  { "lovr.filesystem", luaopen_lovr_filesystem },
#endif
#ifdef LOVR_ENABLE_GRAPHICS
  { "lovr.graphics", luaopen_lovr_graphics },
#endif
#ifdef LOVR_ENABLE_HEADSET
  { "lovr.headset", luaopen_lovr_headset },
#endif
#ifdef LOVR_ENABLE_MATH
  { "lovr.math", luaopen_lovr_math },
#endif
#ifdef LOVR_ENABLE_MOUSE
  { "lovr.mouse", luaopen_lovr_mouse },
#endif
#ifdef LOVR_ENABLE_PHYSICS
  { "lovr.physics", luaopen_lovr_physics },
#endif
#ifdef LOVR_ENABLE_THREAD
  { "lovr.thread", luaopen_lovr_thread },
#endif
#ifdef LOVR_ENABLE_TIMER
  { "lovr.timer", luaopen_lovr_timer },
#endif
#ifdef LOVR_ENABLE_JSON
  { "cjson", luaopen_cjson },
#endif
#ifdef LOVR_ENABLE_ENET
  { "enet", luaopen_enet },
#endif
#ifdef LOVR_ENABLE_UTF8
  { "utf8", luaopen_compat53_utf8 },
#endif
#ifdef LOVR_ENABLE_XXHASH
  { "xxhash", luaopen_xxhash },
#endif
#ifdef LOVR_ENABLE_LUA_BITOP
  /* used when building with Lua 5.1 to provide bit.* library, like LuaJIT */
  { "bit", luaopen_bit },
#endif
  { NULL, NULL }
};

static int l_lovrGetOS(lua_State* L) {
  lua_pushstring(L, lovrPlatformGetName());
  return 1;
}

static int l_lovrGetVersion(lua_State* L) {
  lua_pushinteger(L, LOVR_VERSION_MAJOR);
  lua_pushinteger(L, LOVR_VERSION_MINOR);
  lua_pushinteger(L, LOVR_VERSION_PATCH);
  return 3;
}

static const luaL_Reg lovr[] = {
  { "_setConf", luax_setconf },
  { "getOS", l_lovrGetOS },
  { "getVersion", l_lovrGetVersion },
  { NULL, NULL }
};

LOVR_EXPORT int luaopen_lovr(lua_State* L) {
  lua_newtable(L);
  luax_register(L, lovr);
  return 1;
}
