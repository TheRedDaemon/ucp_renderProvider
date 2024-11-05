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
    using FuncDrawBorderRectangle = void (PencilRenderCore::*)(int x1, int y1, int x2, int y2, unsigned short color);
    using FuncDrawColorRectangle = void (PencilRenderCore::*)(int x1, int y1, int x2, int y2, unsigned short color);
    using FuncDrawDimRectangle = void (PencilRenderCore::*)(int x1, int y1, int x2, int y2);
    using FuncDrawAlphaDimRectangle = void (PencilRenderCore::*)(int x1, int y1, int x2, int y2, int blendStrength);
    using FuncDrawBorderAndDimRectangle = void (PencilRenderCore::*)(int x1, int y1, int width, int height);
    using FuncDrawBorderAndAlphaDimRectangle = void (PencilRenderCore::*)(int x1, int y1, int width, int height, int blendStrength);
    using FuncDrawRoundEdgeAndDimRectangle = void (PencilRenderCore::*)(int x1, int y1, int x2, int y2, RenderProviderHeader::RoundedEdgeType edgeType);
    using FuncDrawRoundEdgeAndColorRectangle = void (PencilRenderCore::*)(int x1, int y1, int x2, int y2, unsigned short color, RenderProviderHeader::RoundedEdgeType edgeType);
  };
}