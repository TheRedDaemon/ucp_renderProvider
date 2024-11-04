module;

#include "renderProviderHeader.h"

export module RenderProvider.SHC:TextManager;

import :GameFont;

export namespace SHC
{
  /* Reduced to the likely relevant data. */
  struct TextManager
  {
    int currentXOffset;
    int currentMultilineYOffset; // could be some interesting data
    RenderProviderHeader::Range textXRange; // start and end of text x-range
    const int codePage;
    const int unknown_0x14;
    const int unknown_0x18;
    RenderProviderHeader::RenderTarget textSurfaceTarget;
    const int unknown_0x20[4];
    int field12_0x30; // BOOL, seems to forces singleline for multiline with proper letter cut off, might be alternative
    const int unknown_0x34[2];
    const int sizeOfOneFontSet; // likely start of parts that are very different in the eastern game versions
    const int alternativeCodePageUsed; // ?
    const int gameLanguageEnum;
    GameFont gameFont[20];
    
    // there is more, but not relevant here

    /* Member functions definitions */
    using FuncRenderSinglelineBlendableTextWithShadow = void (TextManager::*)(const char* textAddress, int xParam, int yParam, RenderProviderHeader::TextAlignment alignment,
      unsigned int primaryColor, unsigned int secondaryColor, RenderProviderHeader::FontSize fontSize, RenderProviderHeader::LeftAlignedTextXOffsetHandling keepOffsetX, int blendStrength);
    using FuncRenderSinglelineBlendableText = void (TextManager::*)(const char* textAddress, int xParam, int yParam, RenderProviderHeader::TextAlignment alignment,
      unsigned int color, RenderProviderHeader::FontSize fontSize, RenderProviderHeader::LeftAlignedTextXOffsetHandling keepOffsetX, int blendStrength);
    using FuncRenderMultilineBlendableTextWithShadow = void (TextManager::*)(const char* textAddress, int xParam, int yParam, int maxWidth,
      unsigned int primaryColor, unsigned int secondaryColor, RenderProviderHeader::FontSize fontSize, int blendStrength);
    using FuncRenderMultilineBlendableText = void (TextManager::*)(const char* textAddress, int xParam, int yParam, int maxWidth,
      unsigned int primaryColor, RenderProviderHeader::FontSize fontSize, int blendStrength);
    

    // utility
    using FuncComputeTextWidth = int (TextManager::*)(const char* text, RenderProviderHeader::FontSize fontSize);
  };
}