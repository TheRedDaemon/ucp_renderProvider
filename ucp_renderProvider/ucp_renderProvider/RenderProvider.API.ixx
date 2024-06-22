module;

#include <stack>

#include "renderProviderHeader.h"

export module RenderProvider.API;

import RenderProvider.Logger;
import RenderProvider.RenderState;

export extern "C" __declspec(dllexport) RenderProviderHeader::FuncRender render;


module :private;

class RenderStateKeeper
{
private:
  static inline std::stack<RenderState> rendererStack{};

public:
  static bool isActiveRenderer(RenderProviderHeader::RenderKey receivedKey)
  {
    if (rendererStack.empty())
    {
      return false;
    }

    return &rendererStack.top() == receivedKey;
  }

private:
  const RenderState& rendererRef;

public:
  RenderStateKeeper() : rendererRef{ rendererStack.emplace() }
  {
  }

  ~RenderStateKeeper()
  {
    rendererStack.pop();
  }

  const RenderState& expose() const
  {
    return rendererRef;
  }

  const RenderProviderHeader::RenderKey exposeAsVoidPtr() const
  {
    return static_cast<const void*>(&rendererRef);
  }
};


extern "C" __declspec(dllexport) void __stdcall render(RenderProviderHeader::RenderTarget target, RenderProviderHeader::FuncRenderAction renderAction, void* misc)
{
  const RenderStateKeeper keeper;
  renderAction(keeper.exposeAsVoidPtr(), misc);
}