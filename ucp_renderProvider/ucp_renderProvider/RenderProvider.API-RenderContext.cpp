module;

#include <memory>
#include <unordered_set>

#include "renderProviderHeader.h"

module RenderProvider.API:RenderContext;

using namespace RenderProviderHeader;

RenderContext::RenderContext()
{

}

RenderContext::~RenderContext() {}

RenderContext& RenderContext::createContext()
{
  auto contextUniquePtr{ std::make_unique<RenderContext>() };
  RenderContext& newContext{ *contextUniquePtr };
  existingContexts.insert(std::move(contextUniquePtr));
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