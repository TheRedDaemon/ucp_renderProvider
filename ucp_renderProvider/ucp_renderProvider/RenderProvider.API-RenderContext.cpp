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

static constexpr Rect GAME_SURFACE_RECT{ 0, 0, 4056 - 1, 2076 - 1 };
static constexpr Rect MIN_SURFACE_RECT{ 0, 0, 800 - 1, 600 - 1 };
static constexpr int BLEND_MAX = 32;

// functions

bool RenderContext::isRectInsideBounds(const Rect& rect, const Rect& bounds)
{
  return rect.limits.left >= bounds.limits.left
    && rect.limits.top >= bounds.limits.top
    && rect.limits.right <= bounds.limits.right
    && rect.limits.bottom <= bounds.limits.bottom;
}

void RenderContext::limitRectToBounds(Rect& rect, const Rect& bounds)
{
  if (rect.limits.left < bounds.limits.left)
  {
    rect.limits.left = bounds.limits.left;
  }
  if (rect.limits.top < bounds.limits.top)
  {
    rect.limits.top = bounds.limits.top;
  }
  if (rect.limits.right > bounds.limits.right)
  {
    rect.limits.right = bounds.limits.right;
  }
  if (rect.limits.bottom > bounds.limits.bottom)
  {
    rect.limits.bottom = bounds.limits.bottom;
  }
}

RenderContext::RenderContext() :
  active{ false },
  target{ RenderTarget::MENU },
  relativeRenderTargetRect{ MIN_SURFACE_RECT }, // set to minimal possible value
  blendStrength{ 0 },
  fontSize{ FontSize::MEDIUM },
  textAlignment{ TextAlignment::LEFT },
  textPrimaryColor{ 0 },
  textSecondaryColor( 0xffffff ),
  textShadow{ false },
  textMultiline{ false },
  textWidth{ 10000 },
  pencilColor{ 0 }
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
  if (reinterpret_cast<const int>(lastUsedContext) == renderer)
  {
    return *lastUsedContext;
  }

  auto it{ existingContexts.find(renderer) };
  if (it == existingContexts.end())
  {
    Log(LogLevel::LOG_FATAL, "[RenderProvider]: Requested render with invalid render context. Critical error. Exiting game.");
    // NO_RETURN: should end the game here via fatal log
  }
  lastUsedContext = it->get();
  return *lastUsedContext;
}

bool RenderContext::removeContext(const RenderContext& context)
{
  auto it{ existingContexts.find(context) };
  if (it == existingContexts.end())
  {
    Log(LogLevel::LOG_FATAL, "[RenderProvider]: Requested removal of not existing render context. Critical error. Exiting game.");
    return false;
  }
  if (lastUsedContext == it->get())
  {
    lastUsedContext = nullptr;
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

  this->setRelativeRenderTargetRect(&this->relativeRenderTargetRect); // set/reset
}

void RenderContext::setRelativeRenderTargetRect(const Rect* rect)
{
  if (!rect)
  {
    Log(LogLevel::LOG_ERROR, "[RenderProvider]: Received nullptr rect for 'setRelativeRenderTargetRect'. Ignoring request.");
    return;
  };

  Rect& targetRect{ this->relativeRenderTargetRect };
  targetRect = *rect;

  Rect compareRect;
  receiveScreenRect(&compareRect);
  if (!isRectInsideBounds(targetRect, compareRect))
  {
    Log(LogLevel::LOG_WARNING, "[RenderProvider]: Target rectangle is out of bounds. Adjusting to resolution size.");
    limitRectToBounds(targetRect, compareRect);
  }

  if (!this->active)
  {
    return;
  }

  // TODO: check ranges one day
  // Could adjust render ranges to partially fit render rect, at least the height range
  // since there is no easy way to restrict the width, this behavior would be strange, so at the moment
  // the restriction is set to the screen rect
  // handling would be kinda possible for text, but pencil would already create problems, since the default
  // code only restricts to the surface sizes
  const Rect& restrictionRect{ compareRect };
  switch (this->target)
  {
  case RenderTarget::MENU:
    GameStruct::TextureRenderCore->screenMenuSurfaceHeightRange = { restrictionRect.limits.top, restrictionRect.limits.bottom + 1 };
    GameStruct::TextureRenderCore->renderingRect = {
      restrictionRect.limits.left,
      restrictionRect.limits.top,
      restrictionRect.limits.right + 1,
      restrictionRect.limits.bottom + 1,
    };
    break;
  case RenderTarget::GAME:
    GameStruct::TextureRenderCore->mapGameSurfaceHeightRange = {
      restrictionRect.limits.top + GameStruct::ViewportState->currentCameraOffsetY,
      restrictionRect.limits.bottom + 1 + GameStruct::ViewportState->currentCameraOffsetY,
    };
    GameStruct::TextureRenderCore->renderingRect = {
      restrictionRect.limits.left + GameStruct::ViewportState->currentCameraOffsetX,
      restrictionRect.limits.top + GameStruct::ViewportState->currentCameraOffsetY,
      restrictionRect.limits.right + 1 + GameStruct::ViewportState->currentCameraOffsetX,
      restrictionRect.limits.bottom + 1 + GameStruct::ViewportState->currentCameraOffsetY,
    };
    break;
  default:
    Log(LogLevel::LOG_FATAL, "[RenderProvider]: An invalid render target was set. Can not adjust surface ranges. Exiting game.");
    break;
  }
  // Text-Range should be defined on text render calls
}

void RenderContext::receiveScreenRect(Rect* rectToFill)
{
  if (!rectToFill)
  {
    Log(LogLevel::LOG_ERROR, "[RenderProvider]: Received nullptr rect for 'receiveScreenRect'. Ignoring request.");
    return;
  };
  *rectToFill = { 0, 0, GameStruct::WindowAndDirectDraw->gameResolutionX - 1, GameStruct::WindowAndDirectDraw->gameResolutionY - 1 };
}
void RenderContext::receiveMenuRect(Rect* rectToFill)
{
  if (!rectToFill)
  {
    Log(LogLevel::LOG_ERROR, "[RenderProvider]: Received nullptr rect for 'receiveMenuRect'. Ignoring request.");
    return;
  };

  const int borderWidth = GameStruct::WindowAndDirectDraw->mainMenuBorderWidth;
  const int borderHeight = GameStruct::WindowAndDirectDraw->mainMenuBorderHeight;
  *rectToFill = {
    borderWidth, borderHeight,
    GameStruct::WindowAndDirectDraw->gameResolutionX - borderWidth - 1,
    GameStruct::WindowAndDirectDraw->gameResolutionY - borderHeight - 1
  };
}
void RenderContext::receiveMapRect(Rect* rectToFill)
{
  if (!rectToFill)
  {
    Log(LogLevel::LOG_ERROR, "[RenderProvider]: Received nullptr rect for 'receiveMapRect'. Ignoring request.");
    return;
  };

  *rectToFill = GAME_SURFACE_RECT;
}
Coord RenderContext::calculateAdjustedPosition(const Coord& position) const
{
  if (this->target == RenderTarget::GAME)
  {
    return {
      position.x + this->relativeRenderTargetRect.coords.position.x + GameStruct::ViewportState->currentCameraOffsetX,
      position.y + this->relativeRenderTargetRect.coords.position.y + GameStruct::ViewportState->currentCameraOffsetY
    };
  }
  else if (this->target == RenderTarget::MENU)
  {
    return { position.x + this->relativeRenderTargetRect.coords.position.x, position.y + this->relativeRenderTargetRect.coords.position.y };
  }
  Log(LogLevel::LOG_FATAL, "[RenderProvider]: Requested position adjusted to unknown render target. Exiting game.");
  return position;
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

void RenderContext::setTextPrimaryColor(unsigned int textPrimaryColor)
{
  this->textPrimaryColor = textPrimaryColor;
}
unsigned int RenderContext::getTextPrimaryColor() const
{
  return this->textPrimaryColor;
}

void RenderContext::setTextSecondaryColor(unsigned int textSecondaryColor)
{
  this->textSecondaryColor = textSecondaryColor;
}
unsigned int RenderContext::getTextSecondaryColor() const
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

int RenderContext::receiveLastLeftAlignedTextXOffset() const
{
  return GameStruct::TextManager->currentXOffset;
}

int RenderContext::receiveLastMultilineTextYOffset() const
{
  return GameStruct::TextManager->currentMultilineYOffset;
}

void RenderContext::setTextShadow(bool textShadow)
{
  this->textShadow = textShadow;
}
bool RenderContext::hasTextShadow() const
{
  return this->textShadow;
}

void RenderContext::setTextMultiline(bool textMultiline)
{
  this->textMultiline = textMultiline;
}
bool RenderContext::isTextMultiline() const
{
  return this->textMultiline;
}

void RenderContext::setTextWidth(int textWidth)
{
  this->textWidth = textWidth;
}
int RenderContext::getTextWidth() const
{
  return this->textWidth;
}



void RenderContext::setPencilColor(unsigned int pencilColor)
{
  this->pencilColor = std::invoke(TextureRenderCoreFunction::transform24bitBGRToDisplayFormat, GameStruct::TextureRenderCore, pencilColor);
}
unsigned short RenderContext::getPencilColor() const
{
  return this->pencilColor;
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