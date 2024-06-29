module;

#include <memory>
#include <unordered_set>

#include "renderProviderHeader.h"

export module RenderProvider.API:RenderContext;

import RenderProvider.GamePointer;
import RenderProvider.Logger;

using namespace RenderProviderHeader;

struct RenderContextHash;

export class RenderContext
{
private:
  static std::unordered_set<std::unique_ptr<RenderContext>, RenderContextHash, std::equal_to<>> existingContexts;

public:
  static RenderContext& createContext();
  static RenderContext& verifyValidContext(Renderer renderer);
  static bool removeContext(const RenderContext& context);
  static bool removeContext(Renderer renderer);

private:
  RenderContext();
  virtual ~RenderContext();

public:
  void initRender() const;
  const Renderer asRenderer() const;
};

module :private;

// source: https://www.cppstories.com/2021/heterogeneous-access-cpp20/#how-to-enable-it-for-unordered-containers
struct RenderContextHash
{
  using is_transparent = void;
  [[nodiscard]] size_t operator()(std::unique_ptr<RenderContext> contextPtr) const
  {
    return std::hash<void*>{}(contextPtr.get());
  }
  [[nodiscard]] size_t operator()(RenderContext& contextPtr) const
  {
    return std::hash<void*>{}(&contextPtr);
  }
  [[nodiscard]] size_t operator()(Renderer renderer) const
  {
    return std::hash<void*>{}(reinterpret_cast<void*>(renderer));
  }
};


RenderContext::RenderContext()
{

}

RenderContext::~RenderContext() {}

RenderContext& RenderContext::createContext()
{
  auto contextUniquePtr{ std::make_unique<RenderContext>() };
  RenderContext& newContext{ *contextUniquePtr };
  existingContexts.insert(contextUniquePtr);
  return newContext;
}

RenderContext& RenderContext::verifyValidContext(Renderer renderer)
{
  auto it{ existingContexts.find(renderer) };
  if (it == existingContexts.end())
  {
    Log(LogLevel::LOG_FATAL, "[RenderProvider]: Requested render with invalid render context. Critical error. Exiting game.");
    // NO_RETURN: should end the game here via fatal log
  }
  return *(it->get());
}

bool RenderContext::removeContext(const RenderContext& context)
{
  auto it{ existingContexts.find(context) };
  if (it == existingContexts.end())
  {
    Log(LogLevel::LOG_FATAL, "[RenderProvider]: Requested removal of not existing render context. Critical error. Exiting game.");
    return false;
  }
  existingContexts.erase(it);
  return true;
}

bool RenderContext::removeContext(Renderer renderer)
{
  return removeContext(*reinterpret_cast<RenderContext*>(renderer));
}

const Renderer RenderContext::asRenderer() const
{
  return reinterpret_cast<Renderer>(this);
}

void RenderContext::initRender() const
{
  // TODO: init context of the game
}