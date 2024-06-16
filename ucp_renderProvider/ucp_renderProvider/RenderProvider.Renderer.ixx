export module RenderProvider.Renderer;

import RenderProvider.Utility;
import RenderProvider.SHC;
import RenderProvider.GamePointer;

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

module :private;

namespace Renderer
{
  Renderer::Renderer() :
    originalDrawBufferChoiceValue{ GameStruct::TextureRenderCore->drawBufferChoiceValue },
    originalCurrentRenderSurfaceIdentifier{ GameStruct::TextureRenderCore->currentRenderSurfaceIdentifier },
    originalTextSurfaceTarget{ GameStruct::TextManager->textSurfaceTarget },
    originalPencilSurfaceTarget{ GameStruct::PencilRenderCore->pencilSurfaceTarget },
    originalMapGameSurfaceHeightRange{ GameStruct::TextureRenderCore->mapGameSurfaceHeightRange },
    originalScreenMenuSurfaceHeightRange{ GameStruct::TextureRenderCore->screenMenuSurfaceHeightRange },
    originalTextXRange{ GameStruct::TextManager->textXRange },
    originalRenderingRect{ GameStruct::TextureRenderCore->renderingRect }
  {
  }

  Renderer::~Renderer()
  {
    GameStruct::TextureRenderCore->drawBufferChoiceValue = originalDrawBufferChoiceValue;
    GameStruct::TextureRenderCore->currentRenderSurfaceIdentifier = originalCurrentRenderSurfaceIdentifier;
    GameStruct::TextManager->textSurfaceTarget = originalTextSurfaceTarget;
    GameStruct::PencilRenderCore->pencilSurfaceTarget = originalPencilSurfaceTarget;
    GameStruct::TextureRenderCore->mapGameSurfaceHeightRange = originalMapGameSurfaceHeightRange;
    GameStruct::TextureRenderCore->screenMenuSurfaceHeightRange = originalScreenMenuSurfaceHeightRange;
    GameStruct::TextManager->textXRange = originalTextXRange;
    GameStruct::TextureRenderCore->renderingRect = originalRenderingRect;
  }
}