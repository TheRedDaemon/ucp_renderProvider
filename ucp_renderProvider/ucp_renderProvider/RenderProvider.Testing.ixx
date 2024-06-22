module;

#include "renderProviderHeader.h"

export module RenderProvider.Testing;

import RenderProvider.SHC;
import RenderProvider.API;

export struct FakeTextureRenderCore
{
  using ActualMenuToMapSurface = void (FakeTextureRenderCore::*)();

  inline static ActualMenuToMapSurface actualMenuToMapSurface{ nullptr };

  // funcs

  void __thiscall detouredMenuToMapSurface();
};

RenderProviderHeader::FuncRenderAction testAction;

module :private;

void __stdcall testAction(RenderProviderHeader::RenderKey key, void* nothing)
{
}

void FakeTextureRenderCore::detouredMenuToMapSurface()
{
  // Testing:
  render(RenderProviderHeader::RenderTarget::GAME, testAction, nullptr);
 

  // Run actual function
  (*this.*actualMenuToMapSurface)();
}