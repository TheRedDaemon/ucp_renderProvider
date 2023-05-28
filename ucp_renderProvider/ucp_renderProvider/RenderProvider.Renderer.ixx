export module RenderProvider.Renderer;

import RenderProvider.Utility;
import RenderProvider.SHC;

export namespace Renderer
{
  class Renderer
  {
  private:
    SHC::RenderTarget originalDrawBufferChoiceValue;
    SHC::RenderTarget originalCurrentRenderSurfaceIdentifier;
    SHC::RenderTarget originalTextSurfaceTarget;
    SHC::RenderTarget originalPencilSurfaceTarget;
    Utility::Range originalMapGameSurfaceHeightRange;
    Utility::Range originalScreenMenuSurfaceHeightRange;
    Utility::Range originalTextXRange;
    Utility::Rect originalRenderingRect;

  public:
    Renderer();
    virtual ~Renderer();
  };
}