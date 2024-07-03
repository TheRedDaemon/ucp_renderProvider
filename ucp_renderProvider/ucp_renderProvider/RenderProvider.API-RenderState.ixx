module;

#include <vector>

#include "renderProviderHeader.h"

export module RenderProvider.API:RenderState;

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
  static RenderState& verifyActiveToken(RenderToken token);
  static bool isContextUsed(const RenderContext& context);
  static bool isContextUsed(Renderer renderer);

private:
  const RenderProviderHeader::RenderTarget originalDrawBufferChoiceValue;
  const RenderProviderHeader::RenderTarget originalCurrentRenderSurfaceIdentifier;
  const RenderProviderHeader::RenderTarget originalTextSurfaceTarget;
  const RenderProviderHeader::RenderTarget originalPencilSurfaceTarget;

  // are not of much use, since the x range seems not clamped, likely need to be set to max value during draw
  const Range originalMapGameSurfaceHeightRange;
  const Range originalScreenMenuSurfaceHeightRange;

  // has an effect on text rendering, although the nature is unclear, it seems to reduce the X range of text
  const Range originalTextXRange;

  // needs to be set, but seems to be only for tgx menu rendering
  const Rect originalRenderingRect;

  const RenderContext& renderContext;

public:
  RenderState(const RenderContext& renderContext);
  ~RenderState();

  const RenderContext& getRenderContext() const;
  const RenderToken asRenderToken() const;
  const Renderer getRenderContextAsRenderer() const;
};
