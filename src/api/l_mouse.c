#include "api.h"
#include "core/os.h"
#include "data/textureData.h"

/* (!) this module does NOT convert between framebuffer & window coords! */

/*
  Missing vs lovr-mouse.lua:
  - getRelativeMode
  - getScale
  - getX, getY, setX, setY
  - setPosition
*/

/* TODO: Destroy cursors on GC? Proper 'cursor' objects? */

StringEntry lovrMouseStandardCursor[] = {
  [MOUSE_SC_ARROW] = ENTRY("arrow"),
  [MOUSE_SC_CROSSHAIR] = ENTRY("crosshair"),
  [MOUSE_SC_HAND] = ENTRY("hand"),
  [MOUSE_SC_IBEAM] = ENTRY("ibeam"),
  [MOUSE_SC_SIZENS] = ENTRY("sizens"),
  [MOUSE_SC_SIZEWE] = ENTRY("sizewe"),
  { 0 }
};

static int l_lovrMouseDestroyCursor(lua_State* L) {
  void* cursorObject = lua_touserdata(L, 1);
  if (cursorObject) {
    lovrPlatformDestroyMouseCursor(cursorObject);
  }
  return 0;
}

static int l_lovrMouseGetPosition(lua_State* L) {
  double x = 0;
  double y = 0;
  lovrPlatformGetMousePosition(&x, &y);

  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  return 2;
}

static int l_lovrMouseGetSystemCursor(lua_State* L) {
  MouseStandardCursor msc = luax_checkenum(L, 1, MouseStandardCursor, "arrow");
  void* cursorObject = lovrPlatformCreateMouseStandardCursor(msc);
  lua_pushlightuserdata(L, cursorObject);
  return 1;
}

static int l_lovrMouseIsDown(lua_State* L) {
  int button = luaL_checkinteger(L, 1) - 1;
  lua_pushboolean(L, lovrPlatformIsMouseDown(button));
  return 1;
}

static int l_lovrMouseNewCursor(lua_State* L) {
  TextureData* textureData = luax_checktype(L, 1, TextureData);
  int hotX = luaL_checkinteger(L, 2);
  int hotY = luaL_checkinteger(L, 3);

  void* cursorObject = lovrPlatformCreateMouseCursor(textureData->source->data, textureData->width, textureData->height, hotX, hotY);

  lua_pushlightuserdata(L, cursorObject);
  return 1;
}

static int l_lovrMouseSetCursor(lua_State* L) {
  if (lua_isnoneornil(L, 1)) {
    lovrPlatformSetMouseCursor(NULL);
    return 0;
  }
  if (lua_islightuserdata(L, 1)) {
    void* cursorObject = lua_touserdata(L, 1);
    lovrPlatformSetMouseCursor(cursorObject);
    return 0;
  }
  return luaL_argerror(L, 1, "expected cursor object lightuserdata");
}

static int l_lovrMouseSetRelativeMode(lua_State* L) {
  bool b = lua_toboolean(L, 1);
  lovrPlatformSetMouseMode(b ? MOUSE_MODE_GRABBED : MOUSE_MODE_NORMAL);
  return 0;
}

static const luaL_Reg lovrMouse[] = {
  { "destroyCursor", l_lovrMouseDestroyCursor },
  { "getPosition", l_lovrMouseGetPosition },
  { "getSystemCursor", l_lovrMouseGetSystemCursor },
  { "isDown", l_lovrMouseIsDown },
  { "newCursor", l_lovrMouseNewCursor },
  { "setCursor", l_lovrMouseSetCursor },
  { "setRelativeMode", l_lovrMouseSetRelativeMode },
  { NULL, NULL }
};

int luaopen_lovr_mouse(lua_State* L) {
  lua_newtable(L);
  luax_register(L, lovrMouse);
  return 1;
}
