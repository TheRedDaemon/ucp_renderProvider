module;

#include <algorithm>

#include "renderProviderHeader.h"

module RenderProvider.API:RenderState;

using namespace RenderProviderHeader;

RenderState::RenderState(const RenderContext& renderContext) :
  originalDrawBufferChoiceValue{ GameStruct::TextureRenderCore->drawBufferChoiceValue },
  originalCurrentRenderSurfaceIdentifier{ GameStruct::TextureRenderCore->currentRenderSurfaceIdentifier },
  originalTextSurfaceTarget{ GameStruct::TextManager->textSurfaceTarget },
  originalPencilSurfaceTarget{ GameStruct::PencilRenderCore->pencilSurfaceTarget },
  originalMapGameSurfaceHeightRange{ GameStruct::TextureRenderCore->mapGameSurfaceHeightRange },
  originalScreenMenuSurfaceHeightRange{ GameStruct::TextureRenderCore->screenMenuSurfaceHeightRange },
  originalTextXRange{ GameStruct::TextManager->textXRange },
  originalRenderingRect{ GameStruct::TextureRenderCore->renderingRect },
  renderContext{ renderContext }
{
  stateStack.push_back(this);
  renderContext.initRender();
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

  stateStack.pop_back();
}


bool RenderState::verifyActiveToken(RenderToken token)
{
  if (!stateStack.empty() && &stateStack.back() == (const void*) token)
  {
    return true;
  }

  Log(LogLevel::LOG_FATAL, "[RenderProvider]: Requested render without the active token. Render order broken. Exiting game.");
  return false;
}

bool RenderState::isContextUsed(const RenderContext& context)
{
  auto it{ std::find_if(stateStack.rbegin(), stateStack.rend(),
    [&context](const RenderState* state) -> bool
    {
      return &(state->getRenderContext()) == &context;
    })
  };
  return it != stateStack.rend();
}

bool RenderState::isContextUsed(Renderer renderer)
{
  return isContextUsed(*reinterpret_cast<const RenderContext*>(renderer));
}

const RenderContext& RenderState::getRenderContext() const
{
  return renderContext;
}

const RenderToken RenderState::asRenderToken() const
{
  return reinterpret_cast<RenderToken>(this);
}

const Renderer RenderState::getRenderContextAsRenderer() const
{
  return reinterpret_cast<Renderer>(&getRenderContext());
}
