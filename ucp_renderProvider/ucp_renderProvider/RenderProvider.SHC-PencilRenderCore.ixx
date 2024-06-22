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
  };
}