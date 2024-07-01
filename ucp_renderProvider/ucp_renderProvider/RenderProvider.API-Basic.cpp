module;

#include "renderProviderHeader.h"

module RenderProvider.API:Basic;

using namespace RenderProviderHeader;

extern "C" __declspec(dllexport) Renderer __stdcall requestRenderer()
{
  return RenderContext::createContext().asRenderer();
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

extern "C" __declspec(dllexport) Renderer __stdcall receiveRenderer(RenderToken token)
{
  return RenderState::verifyActiveToken(token).getRenderContextAsRenderer();
}
