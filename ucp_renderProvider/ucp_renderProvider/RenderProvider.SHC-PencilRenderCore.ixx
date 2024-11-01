module;

#include "renderProviderHeader.h"

export module RenderProvider.SHC:PencilRenderCore;

export namespace SHC
{
  /* Reduced to the likely relevant data. */
  struct PencilRenderCore
  {
    const int unused[3]; // here unused
    RenderProviderHeader::RenderTarget pencilSurfaceTarget;
    // there is more, but not relevant if used by functions

    /* Member functions definitions */
    using FuncDrawLine = void (PencilRenderCore::*)(int x1, int y1, int x2, int y2, unsigned short color);
    using FuncDrawRectangle = void (PencilRenderCore::*)(int x1, int y1, int x2, int y2, unsigned short color);
    using FuncDrawFilledRectangle = void (PencilRenderCore::*)(int x1, int y1, int x2, int y2, unsigned short color);
  };
}