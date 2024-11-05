module;

#include "renderProviderHeader.h"

export module RenderProvider.GamePointer;

import RenderProvider.SHC;

export struct GameStruct
{
  inline static SHC::WindowAndDirectDraw* WindowAndDirectDraw{ nullptr };
  inline static SHC::TextureRenderCore* TextureRenderCore{ nullptr };
  inline static SHC::PencilRenderCore* PencilRenderCore{ nullptr };
  inline static SHC::TextManager* TextManager{ nullptr };
  inline static SHC::ViewportState* ViewportState{ nullptr };
};

export struct WindowAndDirectDrawFunction
{

};

export struct TextureRenderCoreFunction
{

  // utility
  inline static SHC::TextureRenderCore::FuncTransform24bitBGRToDisplayFormat transform24bitBGRToDisplayFormat{ nullptr };
};

export struct PencilRenderCoreFunction
{
  inline static SHC::PencilRenderCore::FuncDrawLine drawLine{ nullptr };
  inline static SHC::PencilRenderCore::FuncDrawRectangle drawBorderRectangle{ nullptr };
  inline static SHC::PencilRenderCore::FuncDrawFilledRectangle drawColorRectangle{ nullptr };
};

export struct TextManagerDrawFunction
{
  inline static SHC::TextManager::FuncRenderSinglelineBlendableTextWithShadow renderSinglelineBlendableTextWithShadow{ nullptr };
  inline static SHC::TextManager::FuncRenderSinglelineBlendableText renderSinglelineBlendableText{ nullptr };
  inline static SHC::TextManager::FuncRenderMultilineBlendableTextWithShadow renderMultilineBlendableTextWithShadow{ nullptr };
  inline static SHC::TextManager::FuncRenderMultilineBlendableText renderMultilineBlendableText{ nullptr };

  // utility
  inline static SHC::TextManager::FuncComputeTextWidth computeTextWidth{ nullptr };
};
