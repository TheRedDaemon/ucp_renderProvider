export module RenderProvider.Globals;

import RenderProvider.SHC;

export struct GameStruct
{
  inline static SHC::WindowAndDirectDraw* WindowAndDirectDraw{ nullptr };
  inline static SHC::TextureRenderCore* TextureRenderCore{ nullptr };
  inline static SHC::PencilRenderCore* PencilRenderCore{ nullptr };
  inline static SHC::TextManager* TextManager{ nullptr };
};

export struct GameFunction
{

};