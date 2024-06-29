module;

#include <vector>

#include "renderProviderHeader.h"

export module RenderProvider.API:RenderState;

import RenderProvider.Utility;
import RenderProvider.SHC;
import RenderProvider.GamePointer;
import RenderProvider.Logger;

import :RenderContext;

using namespace RenderProviderHeader;

export class RenderState final
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
  ~RenderState();

  const RenderContext& getRenderContext() const;
  const RenderToken asRenderToken() const;
  const Renderer getRenderContextAsRenderer() const;
};
