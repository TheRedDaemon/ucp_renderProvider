module;

#include <algorithm>

#include "renderProviderHeader.h"

module RenderProvider.API:RenderState;

import RenderProvider.SHC;
import RenderProvider.GamePointer;
import RenderProvider.Logger;

using namespace RenderProviderHeader;

RenderState::RenderState(RenderContext& renderContext) : renderContext{ renderContext }
{
  if (stateStack.empty())
  {
    originalDrawBufferChoiceValue = GameStruct::TextureRenderCore->drawBufferChoiceValue;
    originalCurrentRenderSurfaceIdentifier = GameStruct::TextureRenderCore->currentRenderSurfaceIdentifier;
    originalTextSurfaceTarget = GameStruct::TextManager->textSurfaceTarget;
    originalPencilSurfaceTarget = GameStruct::PencilRenderCore->pencilSurfaceTarget;
    originalMapGameSurfaceHeightRange = GameStruct::TextureRenderCore->mapGameSurfaceHeightRange;
    originalScreenMenuSurfaceHeightRange = GameStruct::TextureRenderCore->screenMenuSurfaceHeightRange;
    originalTextXRange = GameStruct::TextManager->textXRange;
    originalLeftAlignedTextXOffset = GameStruct::TextManager->currentXOffset;
    originalMultilineYOffset = GameStruct::TextManager->currentMultilineYOffset;
    originalRenderingRect = GameStruct::TextureRenderCore->renderingRect;
  }
  else
  {
    (*stateStack.back()).renderContext.setInactive();
  }
  stateStack.push_back(this);
  renderContext.setActive();
}

RenderState::~RenderState()
{
  (*stateStack.back()).renderContext.setInactive();
  stateStack.pop_back();

  if (stateStack.empty())
  {
    GameStruct::TextureRenderCore->drawBufferChoiceValue = originalDrawBufferChoiceValue;
    GameStruct::TextureRenderCore->currentRenderSurfaceIdentifier = originalCurrentRenderSurfaceIdentifier;
    GameStruct::TextManager->textSurfaceTarget = originalTextSurfaceTarget;
    GameStruct::PencilRenderCore->pencilSurfaceTarget = originalPencilSurfaceTarget;
    GameStruct::TextureRenderCore->mapGameSurfaceHeightRange = originalMapGameSurfaceHeightRange;
    GameStruct::TextureRenderCore->screenMenuSurfaceHeightRange = originalScreenMenuSurfaceHeightRange;
    GameStruct::TextManager->textXRange = originalTextXRange;
    GameStruct::TextManager->currentXOffset = originalLeftAlignedTextXOffset;
    GameStruct::TextManager->currentMultilineYOffset = originalMultilineYOffset;
    GameStruct::TextureRenderCore->renderingRect = originalRenderingRect;
  }
  else
  {
    (*stateStack.back()).renderContext.setActive();
  }
}


RenderState& RenderState::verifyActiveToken(RenderToken token)
{
  if (stateStack.empty() || stateStack.back() != (const void*) token)
  {
    Log(LogLevel::LOG_FATAL, "[RenderProvider]: Requested render without the active token. Render order broken. Exiting game.");
    // NO_RETURN: should end the game here via fatal log
  }
  return *(stateStack.back());
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

RenderContext& RenderState::getRenderContext() const
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
