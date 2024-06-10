module;

#include <string>

#include "windowsInclude.h"
#include "lua.hpp"

export module RenderProvider.Lua;

import RenderProvider.SHC;
import RenderProvider.Globals;

module :private; // prevents re-compilation of other importing modules if the following part is changed

static void addFillAddress(lua_State* L, DWORD fillAddress, const char* name)
{
  static const std::string PREFIX{ "address_" };

  lua_pushinteger(L, fillAddress);
  lua_setfield(L, -2, (PREFIX + name).c_str());
}

// lua module load
extern "C" __declspec(dllexport) int __cdecl luaopen_renderProvider(lua_State * L)
{
  lua_newtable(L); // push a new table on the stack

  // add struct addresses
  addFillAddress(L, (DWORD) &GameStruct::PencilRenderCore, "PencilRenderCore");
  addFillAddress(L, (DWORD) &GameStruct::TextManager, "TextManager");
  addFillAddress(L, (DWORD) &GameStruct::TextureRenderCore, "TextureRenderCore");
  addFillAddress(L, (DWORD) &GameStruct::WindowAndDirectDraw, "WindowAndDirectDraw");

  return 1;
}