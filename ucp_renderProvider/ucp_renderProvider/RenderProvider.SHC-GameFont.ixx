export module RenderProvider.SHC:GameFont;

export namespace SHC
{
  enum class FontRenderType : int
  {
    COLOR = 0,
    BLENDED_COLOR = 1,
    RAW = 2
  };

  struct GameFont
  {
    int fontGmID_0x0; // the gm id/index of the file used for fonts
    int numberOfFontStartInGm_0x4;
    int fontStartImageId_0x8;
    FontRenderType renderType_0xc;
    int baselineOffset_0x10;
    int lineHeight_0x14;
    int letterSpacing_0x18;
    int whiteSpaceWidth_0x1c;
    int unknown_0x20;
  };
}