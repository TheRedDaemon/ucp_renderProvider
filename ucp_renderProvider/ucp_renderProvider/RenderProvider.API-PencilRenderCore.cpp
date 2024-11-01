module;

#include <functional>

#include "renderProviderHeader.h"

module RenderProvider.API:PencilRenderCore;

import RenderProvider.GamePointer;
import RenderProvider.Logger;

import :RenderState;

using namespace RenderProviderHeader;

extern "C" __declspec(dllexport) void __stdcall drawLine(RenderToken token)
{
  auto& context{ RenderState::verifyActiveToken(token).getRenderContext() };

  const Coord position{ context.receiveAdjustedPosition() };
  const Coord target{ context.receiveAdjustedTargetPosition() };
  const unsigned short color{ context.getPencilColor() };

  std::invoke(PencilRenderCoreFunction::drawLine, GameStruct::PencilRenderCore, position.x, position.y, target.x, target.y, color);
}

extern "C" __declspec(dllexport) void __stdcall drawRectangle(RenderToken token, bool fill)
{
  auto& context{ RenderState::verifyActiveToken(token).getRenderContext() };

  const Coord position{ context.receiveAdjustedPosition() };
  const Coord target{ context.receiveAdjustedTargetPosition() };
  const unsigned short color{ context.getPencilColor() };

  std::invoke(fill ? PencilRenderCoreFunction::drawFilledRectangle : PencilRenderCoreFunction::drawRectangle,
    GameStruct::PencilRenderCore, position.x, position.y, target.x, target.y, color);
}
