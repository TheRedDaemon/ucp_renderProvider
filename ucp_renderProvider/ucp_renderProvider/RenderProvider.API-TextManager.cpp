module;

#include <functional>

#include "renderProviderHeader.h"

module RenderProvider.API:TextManager;

import RenderProvider.GamePointer;
import RenderProvider.Logger;

import :RenderState;

using namespace RenderProviderHeader;

extern "C" __declspec(dllexport) void __stdcall renderText(RenderToken token, const char* text)
{
  auto& context{ RenderState::verifyActiveToken(token).getRenderContext() };

  Coord position{ context.receiveAdjustedPosition() };

  const TextAlignment alignment{ context.getTextAlignment() };
  const bool hasShadow{ context.hasTextShadow() };
  const bool isMultiline{ context.isTextMultiline() };

  // for the shadow call, the position is used for the secondary color, lets adjust this to the primary and alignment
  // depending on the range, the shadow or the primary color might be missing
  switch (alignment)
  {
  case TextAlignment::LEFT:
    position.x += hasShadow ? 2 : 0;
    GameStruct::TextManager->textXRange = { position.x, position.x + context.getTextWidth() };
    break;
  case TextAlignment::CENTER:
    {
      const int shadowAdjust{ hasShadow ? 1 : 0 };
      position.x += shadowAdjust;
      
      // the range for center works rather strange and might use something like the middle index
      // TODO?: this solution might be good enough, but if issues arise, then this might need improvement
      const int halfWidth{ context.getTextWidth() / 2 };
      GameStruct::TextManager->textXRange = { position.x - halfWidth + shadowAdjust, position.x + halfWidth + shadowAdjust };
    }
    break;
  case TextAlignment::RIGHT:
    {
      const int shadowAdjust{ hasShadow ? 2 : 0 };
      GameStruct::TextManager->textXRange = { position.x - context.getTextWidth() - shadowAdjust, position.x - shadowAdjust };
    }
    break;
  }
  position.y += hasShadow ? 1 : 0;

  if (hasShadow)
  {
    if (isMultiline)
    {

    }
    else
    {
      std::invoke(TextManagerDrawFunction::renderSinglelineBlendableTextWithShadow, GameStruct::TextManager, text,
        position.x, position.y, alignment, context.getTextPrimaryColor(), context.getTextSecondaryColor(),
        context.getFontSize(), context.determineTextXOffsetHandling(), context.getBlendStrength());
    }
  }
  else
  {
    if (isMultiline)
    {

    }
    else
    {

    }
  }
}

extern "C" __declspec(dllexport) void __stdcall receiveFontData(FontSize fontSize, FontData* receiver)
{
  if (!receiver)
  {
    Log(LogLevel::LOG_ERROR, "[RenderProvider]: Received nullptr receiver for 'receiveFontData'. Ignoring request.");
    return;
  };
  *receiver = *reinterpret_cast<FontData*>(&(GameStruct::TextManager->gameFont[fontSize].baselineOffset_0x10));
}

extern "C" __declspec(dllexport) int __stdcall computeTextWidth(FontSize fontSize, const char* text)
{
  return std::invoke(TextManagerDrawFunction::computeTextWidth, GameStruct::TextManager, text, fontSize);
}