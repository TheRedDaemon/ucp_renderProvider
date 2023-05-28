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
  };
}