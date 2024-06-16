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

Render::FuncRenderAction testAction;

module :private;

void __stdcall testAction(Render::Renderer renderer, void* nothing)
{
}

void FakeTextureRenderCore::detouredMenuToMapSurface()
{
  // Testing:
  render(Render::RenderTarget::GAME, testAction, nullptr);
 

  // Run actual function
  (*this.*actualMenuToMapSurface)();
}