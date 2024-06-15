module;

#include <string>

#include "windowsInclude.h"
#include "lua.hpp"

export module RenderProvider.Lua;

import RenderProvider.SHC;
import RenderProvider.Globals;
import RenderProvider.Testing;

module :private; // prevents re-compilation of other importing modules if the following part is changed

static void addFillAddress(lua_State* L, DWORD fillAddress, const char* name)
{
  static const std::string PREFIX{ "address_" };

  lua_pushinteger(L, fillAddress);
  lua_setfield(L, -2, (PREFIX + name).c_str());
}

static void addMemberFunctionAddress(lua_State* L, auto memberFunctionAddress, const char* name)
{
  static const std::string PREFIX{ "funcAddress_" };

  lua_pushinteger(L, *(DWORD*) &memberFunctionAddress);
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

  // add addresses for testing
  addFillAddress(L, (DWORD) &FakeTextureRenderCore::actualMenuToMapSurface, "ActualMenuToMapSurface");
  addMemberFunctionAddress(L, &FakeTextureRenderCore::detouredMenuToMapSurface, "DetouredMenuToMapSurface");

  return 1;
}