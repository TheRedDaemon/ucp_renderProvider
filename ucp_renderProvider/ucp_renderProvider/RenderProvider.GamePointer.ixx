export module RenderProvider.GamePointer;

import RenderProvider.SHC;

export struct GameStruct
{
  inline static SHC::WindowAndDirectDraw* WindowAndDirectDraw{ nullptr };
  inline static SHC::TextureRenderCore* TextureRenderCore{ nullptr };
  inline static SHC::PencilRenderCore* PencilRenderCore{ nullptr };
  inline static SHC::TextManager* TextManager{ nullptr };
};

export struct WindowAndDirectDrawFunction
{

};

export struct TextureRenderCoreFunction
{

};

export struct PencilRenderCoreFunction
{

};

export struct TextManagerDrawFunction
{

};
