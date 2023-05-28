export module RenderProvider.SHC:PencilRenderCore;

import :RenderTarget;

export namespace SHC
{
  /* Reduced to the likely relevant data. */
  struct PencilRenderCore
  {
    const int unused[3]; // here unused
    RenderTarget pencilSurfaceTarget;
    // there is more, but not relevant if used by functions
  };
}