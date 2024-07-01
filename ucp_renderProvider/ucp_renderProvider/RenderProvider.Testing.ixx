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

void __stdcall testAction(RenderProviderHeader::RenderToken token, void* nothing)
{
  RenderProviderHeader::Renderer renderer{ receiveRenderer(token)};
  //const int size = computeGameTextWidth(key, 1, 1, static_cast<RenderProviderHeader::FontSize>(0));
  //const int size2 = computeTextWidth(key, "1, 1", static_cast<RenderProviderHeader::FontSize>(0));
  //renderGameInGameText(key, 1, 15, 10, 10, RenderProviderHeader::TextAlignment::LEFT, 0xb8e6f5, 0, static_cast<RenderProviderHeader::FontSize>(0x12), RenderProviderHeader::TextXOffsetHandling::DISCARD, 0);
  //// TODO: gather font values
}

void FakeTextureRenderCore::detouredMenuToMapSurface()
{
  // Testing:
  RenderProviderHeader::Renderer renderer{ requestRenderer() };
  render(renderer, testAction, nullptr);
  releaseRenderer(renderer);
 

  // Run actual function
  (*this.*actualMenuToMapSurface)();
}