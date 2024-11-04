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

extern "C" __declspec(dllexport) void __stdcall drawRectangle(RenderToken token, bool fill, const Rect* rect)
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

  std::invoke(fill ? PencilRenderCoreFunction::drawFilledRectangle : PencilRenderCoreFunction::drawRectangle,
    GameStruct::PencilRenderCore, position.x, position.y, target.x, target.y, color);
}
