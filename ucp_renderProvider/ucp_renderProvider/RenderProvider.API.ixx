module;

#include <stack>

#include "renderProviderHeader.h"

export module RenderProvider.API;

import RenderProvider.Logger;
import RenderProvider.Renderer;

export namespace Render
{
  using RenderProviderHeader::Renderer;
  using RenderProviderHeader::RenderTarget;
  using RenderProviderHeader::FuncRenderAction;
}

export extern "C" __declspec(dllexport) RenderProviderHeader::FuncRender render;


module :private;

class RendererKeeper
{
private:
  static inline std::stack<Renderer::Renderer> rendererStack{};

public:
  static bool isActiveRenderer(RenderProviderHeader::Renderer receivedRenderer)
  {
    if (rendererStack.empty())
    {
      return false;
    }

    return &rendererStack.top() == receivedRenderer;
  }

private:
  const Renderer::Renderer& rendererRef;

public:
  RendererKeeper() : rendererRef{ rendererStack.emplace() }
  {
  }

  ~RendererKeeper()
  {
    rendererStack.pop();
  }

  const Renderer::Renderer& expose() const
  {
    return rendererRef;
  }

  const RenderProviderHeader::Renderer exposeAsVoidPtr() const
  {
    return static_cast<const void*>(&rendererRef);
  }
};


extern "C" __declspec(dllexport) void __stdcall render(RenderProviderHeader::RenderTarget target, RenderProviderHeader::FuncRenderAction renderAction, void* misc)
{
  const RendererKeeper keeper;
  renderAction(keeper.exposeAsVoidPtr(), misc);
}