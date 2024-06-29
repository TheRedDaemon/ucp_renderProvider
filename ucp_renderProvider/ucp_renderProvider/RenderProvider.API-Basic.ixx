module;

#include "renderProviderHeader.h"

export module RenderProvider.API:Basic;

import :RenderContext;
import :RenderState;

import RenderProvider.Logger;

using namespace RenderProviderHeader;

export extern "C" __declspec(dllexport) FuncRequestRenderer requestRenderer;
export extern "C" __declspec(dllexport) FuncReleaseRenderer releaseRenderer;

export extern "C" __declspec(dllexport) FuncRender render;

module :private;

extern "C" __declspec(dllexport) Renderer __stdcall requestRenderer()
{
  return RenderContext.createContext().asRenderer();
}

extern "C" __declspec(dllexport) void __stdcall releaseRenderer(Renderer renderer)
{
  if (RenderState::isContextUsed(renderer))
  {
    Log(LogLevel::LOG_FATAL, "[RenderProvider]: Received request to release used render context. Exiting game.");
    return;
  }
  RenderContext::removeContext(renderer);
}

extern "C" __declspec(dllexport) void __stdcall render(Renderer renderer, FuncRenderAction renderAction, void* misc)
{
  const RenderState state{ RenderContext::verifyValidContext(renderer) };
  renderAction(state.asRenderToken(), misc);
}