export module RenderProvider.Testing;

import RenderProvider.SHC;

export struct FakeTextureRenderCore
{
  using ActualMenuToMapSurface = void (FakeTextureRenderCore::*)();

  inline static ActualMenuToMapSurface actualMenuToMapSurface{ nullptr };

  // funcs

  void __thiscall detouredMenuToMapSurface();
};

module :private;

void FakeTextureRenderCore::detouredMenuToMapSurface()
{
  // Testing:


  // Run actual function
  (*this.*actualMenuToMapSurface)();
}