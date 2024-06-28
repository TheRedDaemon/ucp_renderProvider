module;

#include "windowsInclude.h"
#include "lua.hpp"

export module RenderProvider.Lua;

import RenderProvider.SHC;
import RenderProvider.GamePointer;
import RenderProvider.Testing;

module :private; // prevents re-compilation of other importing modules if the following part is changed

static void addAddress(lua_State* L, DWORD address, const char* name)
{
  lua_pushinteger(L, address);
  lua_setfield(L, -2, name);
}

static void addMemberAddress(lua_State* L, auto memberAddress, const char* name)
{
  lua_pushinteger(L, *(DWORD*) &memberAddress);
  lua_setfield(L, -2, name);
}

// lua module load
extern "C" __declspec(dllexport) int __cdecl luaopen_renderProvider(lua_State * L)
{
  lua_newtable(L); // push a new table on the stack

  // push game ptr fill address table
  lua_newtable(L);

  // add struct addresses
  addAddress(L, (DWORD) &GameStruct::PencilRenderCore, "PencilRenderCore");
  addAddress(L, (DWORD) &GameStruct::TextManager, "TextManager");
  addAddress(L, (DWORD) &GameStruct::TextureRenderCore, "TextureRenderCore");
  addAddress(L, (DWORD) &GameStruct::WindowAndDirectDraw, "WindowAndDirectDraw");

  // add func addresses
  addAddress(L, (DWORD) &TextManagerDrawFunction::renderGameInGameText, "RenderGameInGameText");

  addAddress(L, (DWORD) &TextManagerDrawFunction::computeGameTextWidth, "ComputeGameTextWidth");
  addAddress(L, (DWORD) &TextManagerDrawFunction::computeTextWidth, "ComputeTextWidth");
  
  lua_setfield(L, -2, "gamePtr"); // add table to table

  // add addresses for testing
  addAddress(L, (DWORD) &FakeTextureRenderCore::actualMenuToMapSurface, "address_ActualMenuToMapSurface");
  addMemberAddress(L, &FakeTextureRenderCore::detouredMenuToMapSurface, "funcAddress_DetouredMenuToMapSurface");

  return 1;
}