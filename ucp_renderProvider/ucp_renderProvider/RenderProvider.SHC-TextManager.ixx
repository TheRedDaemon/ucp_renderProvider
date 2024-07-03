module;

#include "renderProviderHeader.h"

export module RenderProvider.SHC:TextManager;

export namespace SHC
{
  /* Reduced to the likely relevant data. */
  struct TextManager
  {
    const int currentXOffset;
    const int unknown_0x4; // could be some interesting data
    RenderProviderHeader::Range textXRange; // ?; likely start and end of text x-range
    const int codePage;
    const int unknown_0x14;
    const int unknown_0x18;
    RenderProviderHeader::RenderTarget textSurfaceTarget;
    const int unknown_0x20[4];
    int field12_0x30;         // is some kind of BOOL related to multi-line text I think
    // there is more, but not relevant here

    /* Member functions definitions */
    using FuncRenderGameInGameText = void (TextManager::*)(int textOffsetIndex, int textNumInGroup, int xParam, int yParam, RenderProviderHeader::TextAlignment alignment,
      unsigned int color1, unsigned int color2, RenderProviderHeader::FontSize fontSize, RenderProviderHeader::TextXOffsetHandling keepOffsetX, int blendStrength);
    using FuncRenderInGameText = void (TextManager::*)(char* textAddress, int xParam, int yParam, RenderProviderHeader::TextAlignment alignment,
      unsigned int color1, unsigned int color2, RenderProviderHeader::FontSize fontSize, RenderProviderHeader::TextXOffsetHandling keepOffsetX, int blendStrength);
    //renderMultilineText ? : 00424500
    //renderMultilineText ? : 00473a70
    //renderMultilineText ? : 00424580
    //renderMultilineText ? : 00473ac0
    //renderNumber : 00474390
    //renderNumberToScreen: 00474430
    //renderTextFromTextGroup : 00424470
    //renderTextToScreen : 00474250

    // utility
    using FuncComputeGameTextWidth = int (TextManager::*)(int textOffsetIndex, int textNumInGroup, RenderProviderHeader::FontSize fontSize);
    using FuncComputeTextWidth = int (TextManager::*)(const char* text, RenderProviderHeader::FontSize fontSize);
  };
}