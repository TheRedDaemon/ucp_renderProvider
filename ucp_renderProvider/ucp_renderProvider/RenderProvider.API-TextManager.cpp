module;

#include <functional>

#include "renderProviderHeader.h"

module RenderProvider.API:TextManager;

import RenderProvider.GamePointer;

import :RenderState;

using namespace RenderProviderHeader;

extern "C" __declspec(dllexport) void __stdcall renderText(RenderToken token, const char* text)
{
  RenderContext& context{ RenderState::verifyActiveToken(token).getRenderContext() };



  // TODO: adjust to other calls based on the context values

  Coord position{ context.receiveAdjustedPosition() };
  // TODO: check text range computation + consider alignment!
  //GameStruct::TextManager->textXRange = { position.x, position.x + context.getTextWidth() };
  
  // for the shadow call, the position is used for the secondary color, lets adjust this to the primary and alignment
  // TODO: test actual adjustments
  const TextAlignment alignment{ context.getTextAlignment() };
  switch (alignment)
  {
  case TextAlignment::LEFT:
    position.x += 2;
    break;
  case TextAlignment::CENTER:
    position.x += 1; // Test adjustment here
    break;
  case TextAlignment::RIGHT:
    break;
  }
  position.y += 1;
  std::invoke(TextManagerDrawFunction::renderSinglelineBlendableTextWithShadow, GameStruct::TextManager, text,
    position.x, position.y, alignment, context.getTextPrimaryColor(), context.getTextSecondaryColor(),
    context.getFontSize(), context.getTextXOffsetHandling(), context.getBlendStrength());
}


extern "C" __declspec(dllexport) int __stdcall computeTextWidth(FontSize fontSize, const char* text)
{
  return std::invoke(TextManagerDrawFunction::computeTextWidth, GameStruct::TextManager, text, fontSize);
}