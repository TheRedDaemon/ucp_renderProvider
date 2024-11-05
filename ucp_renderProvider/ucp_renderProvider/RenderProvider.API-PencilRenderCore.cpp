module;

#include <functional>

#include "renderProviderHeader.h"

module RenderProvider.API:PencilRenderCore;

import RenderProvider.GamePointer;
import RenderProvider.Logger;

import :RenderState;

using namespace RenderProviderHeader;

extern "C" __declspec(dllexport) void __stdcall drawLine(RenderToken token, const Rect* fromTo)
{
  auto& context{ RenderState::verifyActiveToken(token).getRenderContext() };
  if (!fromTo)
  {
    Log(LogLevel::LOG_WARNING, "[RenderProvider]: Received nullptr fromTo rect for 'drawLine'. Ignoring request.");
    return;
  }

  const Coord position{ context.calculateAdjustedPosition((*fromTo).coords.position) };
  const Coord target{ context.calculateAdjustedPosition((*fromTo).coords.target) };
  const unsigned short color{ context.getPencilColor() };

  std::invoke(PencilRenderCoreFunction::drawLine, GameStruct::PencilRenderCore, position.x, position.y, target.x, target.y, color);
}

extern "C" __declspec(dllexport) void __stdcall drawRectangle(RenderToken token, RectangleType type, const Rect* rect)
{
  auto& context{ RenderState::verifyActiveToken(token).getRenderContext() };
  if (!rect)
  {
    Log(LogLevel::LOG_WARNING, "[RenderProvider]: Received nullptr position rect for 'drawRectangle'. Ignoring request.");
    return;
  }

  const Coord position{ context.calculateAdjustedPosition((*rect).coords.position) };
  const Coord target{ context.calculateAdjustedPosition((*rect).coords.target) };
  const unsigned short color{ context.getPencilColor() };
  const int blendStrength{ context.getBlendStrength() };

  switch (type)
  {
  case RenderProviderHeader::DIM:
    break;
  case RenderProviderHeader::BORDER:
    std::invoke(PencilRenderCoreFunction::drawBorderRectangle, GameStruct::PencilRenderCore,
      position.x, position.y, target.x, target.y, color);
    break;
  case RenderProviderHeader::COLOR:
    std::invoke(PencilRenderCoreFunction::drawColorRectangle, GameStruct::PencilRenderCore,
      position.x, position.y, target.x, target.y, color);
    break;
  case RenderProviderHeader::BLEND:
    break;
  case RenderProviderHeader::BORDER_AND_BLEND:
    break;
  case RenderProviderHeader::BORDER_AND_ALPHA_BLEND:
    break;
  case RenderProviderHeader::SLIGHT_ROUND_EDGE_AND_DIM:
    break;
  case RenderProviderHeader::STRONG_ROUND_EDGE_AND_DIM:
    break;
  case RenderProviderHeader::SLIGHT_ROUND_EDGE_AND_COLOR:
    break;
  case RenderProviderHeader::STRONG_ROUND_EDGE_AND_COLOR:
    break;
  default:
    Log(LogLevel::LOG_WARNING, "[RenderProvider]: Received unknown rectangle type for 'drawRectangle'. Ignoring request.");
    break;
  }
}
