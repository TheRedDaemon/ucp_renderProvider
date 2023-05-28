export module RenderProvider.SHC:TextureRenderCore;

import RenderProvider.Utility;

import :RenderTarget;

export namespace SHC
{
  enum class GmImageType
  {
    InterfaceElement = 1,
    Animation = 2,
    Tileset = 3,
    Font = 4,
    UncompressedImage = 5,
    CompressedImage = 6,
    UncompressedImage_2 = 7 // ?
  };

  struct GMFileHeaderColorpalette
  {
    int unknown0;
    int unknown1;
    int unknown2;
    int numberOfPicturesInFile;
    int unknown3;
    GmImageType ImageType;
    int unknown4;
    int unknown5;
    int unknown6;
    int unknown7;
    int unknown8;
    int unknown9;
    int width;
    int height;
    int unknown12;
    int unknown13;
    int unknown14;
    int unknown15;
    int originX;
    int originY;
    int dataSize;
    int unknown18;
    short colorPalette[10][256];
  };

  /* Object to manage loaded gfx files. */
  struct GfxRef
  {
    int width;
    int height;
    int offsetInBuffer;
    int backwardsOffsetInBuffer;
  };

  /* Reduced to the likely relevant data. */
  struct TextureRenderCore
  {
    const int unused_0x0;
    RenderTarget drawBufferChoiceValue; // seems to be general enum for most cases (also get override in a lot of cases by the others)
    RenderTarget currentRenderSurfaceIdentifier; // takes precedence in certain situations
    const int unused_0xc[17];
    int unknownGmRelatedFlag; // If set to 1, only one variation of the big menu buttons is used.
    const int unused_0x54;
    Utility::Range mapGameSurfaceHeightRange;
    Utility::Range screenMenuSurfaceHeightRange;
    const int unused_0x68[301];
    GMFileHeaderColorpalette gmFileHeaderColorpaletteArray[240];
    const char unused_0x13179c[240][1001];
    GfxRef loadedGfxArray[100];
    int numOfLoadedGfx;
    int backwardsLoadedGfxIndex;
    Utility::Rect renderingRect;
    // there is more, but seemingly not of use for this module
  };
}