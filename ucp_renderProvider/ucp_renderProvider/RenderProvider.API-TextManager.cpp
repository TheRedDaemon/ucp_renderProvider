module;

#include <functional>

#include "renderProviderHeader.h"

module RenderProvider.API:TextManager;

import RenderProvider.GamePointer;
import RenderProvider.Logger;

import :RenderState;

using namespace RenderProviderHeader;

extern "C" __declspec(dllexport) void __stdcall renderText(RenderToken token, const char* text, const Coord* requestedPosition)
{
  auto& context{ RenderState::verifyActiveToken(token).getRenderContext() };
  if (!requestedPosition)
  {
    Log(LogLevel::LOG_WARNING, "[RenderProvider]: Received nullptr position for 'renderText'. Ignoring request.");
    return;
  }

  // resolve all values to consider side effects
  Coord position{ context.calculateAdjustedPosition(*requestedPosition) };
  const int textWidth{ context.getTextWidth() };
  const TextAlignment alignment{ context.getTextAlignment() };
  const unsigned int primaryColor{ context.getTextPrimaryColor() };
  const unsigned int secondaryColor{ context.getTextSecondaryColor() };
  const FontSize fontSize{ context.getFontSize() };
  const int blendStrength{ context.getBlendStrength() };
  const bool hasShadow{ context.hasTextShadow() };
  const bool isMultiline{ context.isTextMultiline() };

  // for the shadow call, the position is used for the secondary color, lets adjust this to the primary and alignment
  // depending on the text range, the shadow or the primary color might be missing

  if (isMultiline) // only supports left alignment
  {
    GameStruct::TextManager->textXRange = { position.x, position.x + textWidth };
    position.x += hasShadow ? 2 : 0;
    position.y += hasShadow ? 1 : 0;

    if (hasShadow)
    {
      std::invoke(TextManagerDrawFunction::renderMultilineBlendableTextWithShadow, GameStruct::TextManager,
        text, position.x, position.y, textWidth, primaryColor, secondaryColor, fontSize, blendStrength);
    }
    else
    {
      std::invoke(TextManagerDrawFunction::renderMultilineBlendableText, GameStruct::TextManager, text,
        position.x, position.y, textWidth, primaryColor, fontSize, blendStrength);
    }
    return;
  }
 
  switch (alignment)
  {
  case TextAlignment::LEFT:
    GameStruct::TextManager->textXRange = { position.x, position.x + textWidth };
    position.x += hasShadow ? 2 : 0;
    break;
  case TextAlignment::CENTER:
    {
      // the range for center works rather strange and might use something like the middle index
      // TODO?: this solution might be good enough, but if issues arise, then this might need improvement
      const int halfWidth{ textWidth / 2 };
      GameStruct::TextManager->textXRange = { position.x - halfWidth - 1, position.x + halfWidth - 1 };
      position.x += hasShadow ? 1 : -1;
    }
    break;
  case TextAlignment::RIGHT:
    GameStruct::TextManager->textXRange = { position.x - textWidth - 1, position.x - 1 };
    position.x += 2; // adjust for space
    break;
  }
  position.y += hasShadow ? 1 : 0;

  // offset handling is always reset, the user should request the offset to use it in the next call
  if (hasShadow)
  {
    std::invoke(TextManagerDrawFunction::renderSinglelineBlendableTextWithShadow, GameStruct::TextManager, text, position.x,
      position.y, alignment, primaryColor, secondaryColor, fontSize, LeftAlignedTextXOffsetHandling::DISCARD, blendStrength);
  }
  else
  {
    std::invoke(TextManagerDrawFunction::renderSinglelineBlendableText, GameStruct::TextManager, text, position.x,
      position.y, alignment, primaryColor, fontSize, LeftAlignedTextXOffsetHandling::DISCARD, blendStrength);
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
