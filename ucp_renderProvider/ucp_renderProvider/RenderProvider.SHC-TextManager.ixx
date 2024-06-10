export module RenderProvider.SHC:TextManager;

import RenderProvider.Utility;

import :RenderTarget;

export namespace SHC
{
  /* Reduced to the likely relevant data. */
  struct TextManager
  {
    const int currentXOffset;
    const int unknown_0x4; // could be some interesting data
    Utility::Range textXRange; // ?; likely start and end of text x-range
    const int codePage;
    const int unknown_0x14;
    const int unknown_0x18;
    RenderTarget textSurfaceTarget;
    const int unknown_0x20[4];
    int field12_0x30;         // is some kind of BOOL related to multi-line text I think
    // there is more, but not relevant here

    /* Enums */
    enum TextXOffsetHandling : int
    {
      DISCARD = 0,
      KEEP = 1,
    };

    enum TextAlignment : int
    {
      LEFT = 0,
      CENTER = 1,
      RIGHT = -1,
    };

    /* Member functions definitions */
    using FuncRenderGameInGameText = int (TextManager::*)(int textOffsetIndex, int textNumInGroup, int xParam, int yParam, TextAlignment alignment,
      unsigned int color1, unsigned int color2, int fontSize, TextXOffsetHandling keepOffsetX, int blendStrength);
    using FuncRenderInGameText = int (TextManager::*)(char* textAddress, int xParam, int yParam, TextAlignment alignment,
      unsigned int color1, unsigned int color2, int fontSize, TextXOffsetHandling keepOffsetX, int blendStrength);
    //renderMultilineText ? : 00424500
    //renderMultilineText ? : 00473a70
    //renderMultilineText ? : 00424580
    //renderMultilineText ? : 00473ac0
    //renderNumber : 00474390
    //renderNumberToScreen: 00474430
    //renderTextFromTextGroup : 00424470
    //renderTextToScreen : 00474250

    // utility
    using FuncComputeGameTextWidth = int (TextManager::*)(int textOffsetIndex, int textNumInGroup, int fontSize);
    using FuncComputeTextWidth = int (TextManager::*)(const char* text, int fontSize);
  };
}