module;

#include <memory>
#include <algorithm>

#include "renderProviderHeader.h"

module RenderProvider.API;

using namespace RenderProviderHeader;

// TODO: this does not work: the render state and the "state keeper" need to be separated

bool RenderStateKeeper::verifyActiveRenderer(RenderToken token)
{
  if (!stateStack.empty() && &stateStack.back() == (const void*) token)
  {
    return true;
  }

  Log(LogLevel::LOG_FATAL, "[RenderProvider]: Requested render without the active key. Render order broken. Exiting game.");
  return false;
}

bool RenderStateKeeper::checkIfTokenUsed(RenderToken token)
{

  if (!stateStack.empty() && &stateStack.back() == (const void*) token)
  {
    return true;
  }

  Log(LogLevel::LOG_FATAL, "[RenderProvider]: Requested render without the active key. Render order broken. Exiting game.");
  return false;
}

RenderStateKeeper::RenderStateKeeper() : rendererRef{ stateStack.emplace() }
{
}

RenderStateKeeper::~RenderStateKeeper()
{
  stateStack.pop_back();
}

const RenderState& RenderStateKeeper::expose() const
{
  return rendererRef;
}

const RenderToken RenderStateKeeper::exposeAsConstInt() const
{
  return reinterpret_cast<RenderToken>(&rendererRef);
}

extern "C" __declspec(dllexport) RenderToken __stdcall requestRenderToken()
{
  renderStates.push_back(std::make_unique<RenderState>());
  return reinterpret_cast<const int>(renderStates.back().get());
}

extern "C" __declspec(dllexport) void __stdcall releaseRenderToken(RenderToken token)
{
  auto it{ std::find(renderStates.begin(), renderStates.end(),
    [token](const std::unique_ptr<RenderState>& state) -> bool
    {
      return reinterpret_cast<const int>(state.get()) == token;
    })
  };
  
  if (it == renderStates.end())
  {
    Log(LogLevel::LOG_FATAL, "[RenderProvider]: Received request to release non-existing RenderToken. Exiting game.");
    return;
  }


}

