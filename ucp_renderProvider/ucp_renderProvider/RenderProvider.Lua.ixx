module;

#include "windowsInclude.h"
#include "lua.hpp"

export module RenderProvider.Lua;

import RenderProvider.SHC;

module :private; // prevents re-compilation of other importing modules if the following part is changed

SHC::WindowAndDirectDraw* windowAndDirectDraw{ reinterpret_cast<SHC::WindowAndDirectDraw*>(0x00f98338) };
SHC::TextManager* textManager{ reinterpret_cast<SHC::TextManager*>(0x02157578) };
SHC::PencilRenderCore* pencilRenderCore{ reinterpret_cast<SHC::PencilRenderCore*>(0x0191d720) };
SHC::TextureRenderCore* textureRenderCore{ reinterpret_cast<SHC::TextureRenderCore*>(0x01fea090) };

extern "C" __declspec(dllexport) int __cdecl lua_test(lua_State * L)
{
  SHC::WindowAndDirectDraw* test1{ windowAndDirectDraw };
  SHC::TextManager* test2{ textManager };
  SHC::PencilRenderCore* test3{ pencilRenderCore };
  SHC::TextureRenderCore* test4{ textureRenderCore };
  return 1;
}

// lua module load
extern "C" __declspec(dllexport) int __cdecl luaopen_renderProvider(lua_State * L)
{
  lua_newtable(L); // push a new table on the stack

  // return lua funcs
  lua_pushcfunction(L, lua_test);
  lua_setfield(L, -2, "lua_test");

  return 1;
}