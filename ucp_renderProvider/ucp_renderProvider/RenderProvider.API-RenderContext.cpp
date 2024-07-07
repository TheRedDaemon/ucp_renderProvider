module;

#include <memory>
#include <unordered_set>
#include <algorithm>

#include "renderProviderHeader.h"

module RenderProvider.API:RenderContext;

import RenderProvider.Logger;
import RenderProvider.GamePointer;

using namespace RenderProviderHeader;

// constants

static constexpr Rect GAME_MAP_RECT{ 0, 0, 4056, 2076 };
static constexpr int BLEND_MAX = 32;

// functions

bool RenderContext::isRectInsideBounds(const Rect& rect, const Rect& bounds)
{
  return (rect.x >= bounds.x && rect.y >= bounds.y && rect.x + rect.width <= bounds.x + bounds.width && rect.y + rect.height <= bounds.y + bounds.height);
}

void RenderContext::limitRectToBounds(Rect& rect, const Rect& bounds)
{
  if (rect.x < bounds.x)
  {
    rect.width -= bounds.x - rect.x;
    rect.x = bounds.x;
  }
  if (rect.y < bounds.y)
  {
    rect.height -= bounds.y - rect.y;
    rect.y = bounds.y;
  }
  if (rect.x + rect.width > bounds.x + bounds.width)
  {
    rect.width = bounds.x + bounds.width - rect.x;
  }
  if (rect.y + rect.height > bounds.y + bounds.height)
  {
    rect.height = bounds.y + bounds.height - rect.y;
  }
}

RenderContext::RenderContext() :
  active{ false },
  target{ RenderTarget::MENU },
  blendStrength{ BLEND_MAX },
  fontSize{ FontSize::MEDIUM },
  textAlignment{ TextAlignment::LEFT },
  textPrimaryColor{ 0 },
  textSecondaryColor( 0xffffff ),
  requestedTextPositionReset{ true }, // assume first text request to start at position
  textWidth{ 10000 }
{
  this->receiveScreenRect(this->relativeMenuTargetRect);
  this->receiveMapRect(this->relativeGameTargetRect);
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

void RenderContext::setTarget(RenderTarget target)
{
  if (target == RenderTarget::BUTTON_AND_ALPHA || target == RenderTarget::CONTEXT_BASED)
  {
    Log(LogLevel::LOG_WARNING, "[RenderProvider]: Requested invalid render target. Only GAME and MENU are supported. Request ignored.");
    return;
  }
  // set always, do not optimize
  this->target = target;

  if (active)
  {
    GameStruct::TextureRenderCore->drawBufferChoiceValue = target;
    GameStruct::TextureRenderCore->currentRenderSurfaceIdentifier = target;
    GameStruct::TextManager->textSurfaceTarget = target;
    GameStruct::PencilRenderCore->pencilSurfaceTarget = target;
  }

  if (target == RenderTarget::MENU)
  {
    this->setRelativeMenuTargetRect(this->relativeMenuTargetRect); // set/reset
  }
  else if (target == RenderTarget::GAME)
  {
    this->setRelativeGameTargetRect(this->relativeGameTargetRect); // set/reset
  }
}

void RenderContext::setRelativeMenuTargetRect(const Rect& rect)
{
  Rect& targetRect{ this->relativeMenuTargetRect };
  targetRect = rect;

  Rect compareRect;
  receiveScreenRect(compareRect);
  if (!isRectInsideBounds(targetRect, compareRect))
  {
    Log(LogLevel::LOG_WARNING, "[RenderProvider]: Menu target rectangle is out of bounds. Adjusting to screen size.");
    limitRectToBounds(targetRect, compareRect);
  }

  if (this->active && this->target == RenderTarget::MENU)
  {
    // TODO check ranges
    const Range yRange{ targetRect.y, targetRect.y + targetRect.height };
    GameStruct::TextureRenderCore->screenMenuSurfaceHeightRange = yRange;
    GameStruct::TextureRenderCore->renderingRect = targetRect;
    // Text-Range should be defined on text render calls
  }
}

void RenderContext::setRelativeGameTargetRect(const Rect& rect)
{
  Rect& targetRect{ this->relativeGameTargetRect };
  targetRect = rect;
  if (!isRectInsideBounds(targetRect, GAME_MAP_RECT))
  {
    Log(LogLevel::LOG_WARNING, "[RenderProvider]: Game target rectangle is out of bounds. Adjusting to game map size.");
    limitRectToBounds(targetRect, GAME_MAP_RECT);
  }

  if (this->active && this->target == RenderTarget::GAME)
  {
    // TODO check ranges
    const Range yRange{ targetRect.y, targetRect.y + targetRect.height };
    GameStruct::TextureRenderCore->mapGameSurfaceHeightRange = yRange;
    GameStruct::TextureRenderCore->renderingRect = targetRect;
    // Text-Range should be defined on text render calls
  }
}

void RenderContext::receiveScreenRect(Rect& rectToFill)
{
  rectToFill.x = 0;
  rectToFill.y = 0;
  rectToFill.width = GameStruct::WindowAndDirectDraw->gameResolutionX;
  rectToFill.height = GameStruct::WindowAndDirectDraw->gameResolutionY;
}
void RenderContext::receiveMenuRect(Rect& rectToFill)
{
  const int borderWidth = GameStruct::WindowAndDirectDraw->mainMenuBorderWidth;
  const int borderHeight = GameStruct::WindowAndDirectDraw->mainMenuBorderHeight;
  rectToFill.x = borderWidth;
  rectToFill.y = borderHeight;
  rectToFill.width = GameStruct::WindowAndDirectDraw->gameResolutionX - 2 * borderWidth;
  rectToFill.height = GameStruct::WindowAndDirectDraw->gameResolutionY - 2 * borderHeight;
}
void RenderContext::receiveMapRect(Rect& rectToFill)
{
  rectToFill = GAME_MAP_RECT;
}

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

void RenderContext::setTextPrimaryColor(int textPrimaryColor)
{
  this->textPrimaryColor = textPrimaryColor;
}
int RenderContext::getTextPrimaryColor() const
{
  return this->textPrimaryColor;
}

void RenderContext::setTextSecondaryColor(int textSecondaryColor)
{
  this->textSecondaryColor = textSecondaryColor;
}
int RenderContext::getTextSecondaryColor() const
{
  return this->textSecondaryColor;
}

void RenderContext::setTextAlignment(TextAlignment textAlignment)
{
  this->textAlignment = textAlignment;
}
TextAlignment RenderContext::getTextAlignment() const
{
  return this->textAlignment;
}

void RenderContext::requestTextPositionReset()
{
  this->requestedTextPositionReset = true;
}
TextXOffsetHandling RenderContext::getTextXOffsetHandling()
{
  if (this->requestedTextPositionReset)
  {
    this->requestedTextPositionReset = false;
    return TextXOffsetHandling::DISCARD;
  }
  return TextXOffsetHandling::KEEP;
}

void RenderContext::setTextWidth(int textWidth)
{
  this->textWidth = textWidth;
}
int RenderContext::getTextWidth() const
{
  return this->textWidth;
}



const Renderer RenderContext::asRenderer() const
{
  return reinterpret_cast<Renderer>(this);
}

void RenderContext::setActive()
{
  this->active = true;

  // just set target again, should take care of setting the needed values
  setTarget(this->target);
}

void RenderContext::setInactive()
{
  this->active = false;
}