module;

#include <memory>
#include <unordered_set>
#include <algorithm>

#include "renderProviderHeader.h"

module RenderProvider.API:RenderContext;

using namespace RenderProviderHeader;

// constants

static int BLEND_MAX = 32;

// functions

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

// general

void RenderContext::setAlpha(float alpha)
{
  if (alpha < 0.0f || alpha > 1.0f)
  {
    Log(LogLevel::LOG_WARNING, "[RenderProvider]: Requested invalid alpha strength. Clamping to 0.0-1.0.");
    alpha = std::clamp(alpha, 0.0f, 1.0f);
  }
  this->blendStrength = BLEND_MAX - static_cast<int>(BLEND_MAX * alpha);
}
int RenderContext::getBlendStrength() const
{
  return this->blendStrength;
}

// text

void RenderContext::setFontSize(FontSize fontSize)
{
  this->fontSize = fontSize;
}
FontSize RenderContext::getFontSize() const
{
  return this->fontSize;
}

void RenderContext::setFontPrimaryColor(int fontPrimaryColor)
{
  this->fontPrimaryColor = fontPrimaryColor;
}
int RenderContext::getFontPrimaryColor() const
{
  return this->fontPrimaryColor;
}

void RenderContext::setFontSecondaryColor(int fontSecondaryColor)
{
  this->fontSecondaryColor = fontSecondaryColor;
}
int RenderContext::getFontSecondaryColor() const
{
  return this->fontSecondaryColor;
}

void RenderContext::setFontAlignment(TextAlignment textAlignment)
{
  this->textAlignment = textAlignment;
}
TextAlignment RenderContext::getFontAlignment() const
{
  return this->textAlignment;
}



const Renderer RenderContext::asRenderer() const
{
  return reinterpret_cast<Renderer>(this);
}

void RenderContext::initRender() const
{
  // TODO: init context of the game
}