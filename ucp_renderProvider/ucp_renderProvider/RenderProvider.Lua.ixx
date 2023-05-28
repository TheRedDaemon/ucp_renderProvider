module;

#include "windowsInclude.h"
#include "lua.hpp"

export module MenuHandler.Lua;

import MenuHandler.SHC;

module :private; // prevents re-compilation of other importing modules if the following part is changed

extern "C" __declspec(dllexport) int __cdecl lua_test(lua_State * L)
{
  return 1;
}

// lua module load
extern "C" __declspec(dllexport) int __cdecl luaopen_menuHandler(lua_State * L)
{
  lua_newtable(L); // push a new table on the stack

  // return lua funcs
  lua_pushcfunction(L, lua_test);
  lua_setfield(L, -2, "lua_test");

  return 1;
}