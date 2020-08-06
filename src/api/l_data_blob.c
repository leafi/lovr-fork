#include "api.h"
#include "data/blob.h"

#ifdef LOVR_ENABLE_XXHASH
#include "xxhash.h"
#endif

static int l_lovrBlobGetName(lua_State* L) {
  Blob* blob = luax_checktype(L, 1, Blob);
  lua_pushstring(L, blob->name);
  return 1;
}

static int l_lovrBlobGetPointer(lua_State* L) {
  Blob* blob = luax_checktype(L, 1, Blob);
  lua_pushlightuserdata(L, blob->data);
  return 1;
}

static int l_lovrBlobGetSize(lua_State* L) {
  Blob* blob = luax_checktype(L, 1, Blob);
  lua_pushinteger(L, blob->size);
  return 1;
}

static int l_lovrBlobGetString(lua_State* L) {
  Blob* blob = luax_checktype(L, 1, Blob);
  lua_pushlstring(L, blob->data, blob->size);
  return 1;
}

#ifdef LOVR_ENABLE_XXHASH
static int l_lovrBlobXXH32(lua_State* L) {
  Blob* blob = luax_checktype(L, 1, Blob);
  unsigned int seed = luaL_checkinteger(L, 2);
  unsigned int hash = XXH32(blob->data, blob->size, seed);
  lua_pushinteger(L, hash);
  return 1;
}
#endif

const luaL_Reg lovrBlob[] = {
  { "getName", l_lovrBlobGetName },
  { "getPointer", l_lovrBlobGetPointer },
  { "getSize", l_lovrBlobGetSize },
  { "getString", l_lovrBlobGetString },
#ifdef LOVR_ENABLE_XXHASH
  { "xxh32", l_lovrBlobXXH32 },
#endif
  { NULL, NULL }
};
