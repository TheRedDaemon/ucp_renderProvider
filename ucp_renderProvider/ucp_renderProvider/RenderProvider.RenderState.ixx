export module RenderProvider.RenderState;

import RenderProvider.Utility;
import RenderProvider.SHC;
import RenderProvider.GamePointer;


export class RenderState
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
  RenderState();
  virtual ~RenderState();
};

module :private;


RenderState::RenderState() :
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

RenderState::~RenderState()
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