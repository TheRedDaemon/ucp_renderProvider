module;

#include <vector>
#include <functional>

#include "renderProviderHeader.h"

export module RenderProvider.API:RenderState;

import :RenderContext;

using namespace RenderProviderHeader;

export class RenderState final
{
private:
  static inline std::vector<RenderState*> stateStack{};

  // keep original values as static, since in multi renderer contexts the values
  // of the individual contexts are restored by the context when set active

  static inline RenderProviderHeader::RenderTarget originalDrawBufferChoiceValue;
  static inline RenderProviderHeader::RenderTarget originalCurrentRenderSurfaceIdentifier;
  static inline RenderProviderHeader::RenderTarget originalTextSurfaceTarget;
  static inline RenderProviderHeader::RenderTarget originalPencilSurfaceTarget;

  // are not of much use, since the x range seems not clamped, likely need to be set to max value during draw
  static inline Range originalMapGameSurfaceHeightRange;
  static inline Range originalScreenMenuSurfaceHeightRange;

  // has an effect on text rendering, although the nature is unclear, it seems to reduce the X range of text
  static inline Range originalTextXRange;

  // singleline, left aligned text can be put together this way
  static inline int originalLeftAlignedTextXOffset;

  // needs to be set, but seems to be only for tgx menu rendering
  static inline Rect originalRenderingRect;

public:
  static RenderState& verifyActiveToken(RenderToken token);
  static bool isContextUsed(const RenderContext& context);
  static bool isContextUsed(Renderer renderer);

private:
  RenderContext& renderContext;

public:
  RenderState(RenderContext& renderContext);
  ~RenderState();

  RenderContext& getRenderContext() const;
  const RenderToken asRenderToken() const;
  const Renderer getRenderContextAsRenderer() const;
};
