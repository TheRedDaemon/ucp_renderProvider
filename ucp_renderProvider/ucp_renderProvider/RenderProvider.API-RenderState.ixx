module;

#include <vector>
#include <algorithm>

#include "renderProviderHeader.h"

export module RenderProvider.API:RenderState;

import RenderProvider.Utility;
import RenderProvider.SHC;
import RenderProvider.GamePointer;
import RenderProvider.Logger;

import :RenderContext;

using namespace RenderProviderHeader;

export class RenderState
{
private:
  static inline std::vector<RenderState*> stateStack{};

public:
  static bool verifyActiveToken(RenderToken token);
  static bool isContextUsed(const RenderContext& context);
  static bool isContextUsed(Renderer renderer);

private:
  const RenderProviderHeader::RenderTarget originalDrawBufferChoiceValue;
  const RenderProviderHeader::RenderTarget originalCurrentRenderSurfaceIdentifier;
  const RenderProviderHeader::RenderTarget originalTextSurfaceTarget;
  const RenderProviderHeader::RenderTarget originalPencilSurfaceTarget;
  const Utility::Range originalMapGameSurfaceHeightRange;
  const Utility::Range originalScreenMenuSurfaceHeightRange;
  const Utility::Range originalTextXRange;
  const Utility::Rect originalRenderingRect;

  const RenderContext& renderContext;

public:
  RenderState(const RenderContext& renderContext);
  virtual ~RenderState();

  const RenderContext& getRenderContext() const;
  const RenderToken asRenderToken() const;
  const Renderer getRenderContextAsRenderer() const;
};

module :private;


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
  auto it{ std::find(stateStack.rbegin(), stateStack.rend(),
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

const Renderer  RenderState::getRenderContextAsRenderer() const
{
  return reinterpret_cast<Renderer>(&getRenderContext());
}
