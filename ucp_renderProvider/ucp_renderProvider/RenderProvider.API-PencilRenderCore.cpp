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
    std::invoke(PencilRenderCoreFunction::drawDimRectangle, GameStruct::PencilRenderCore,
      position.x, position.y, target.x, target.y);
    break;
  case RenderProviderHeader::BORDER:
    std::invoke(PencilRenderCoreFunction::drawBorderRectangle, GameStruct::PencilRenderCore,
      position.x, position.y, target.x, target.y, color);
    break;
  case RenderProviderHeader::COLOR:
    std::invoke(PencilRenderCoreFunction::drawColorRectangle, GameStruct::PencilRenderCore,
      position.x, position.y, target.x, target.y, color);
    break;
  case RenderProviderHeader::ALPHA_DIM:
    std::invoke(PencilRenderCoreFunction::drawAlphaDimRectangle, GameStruct::PencilRenderCore,
      position.x, position.y, target.x, target.y, blendStrength);
    break;
  case RenderProviderHeader::SLIGHT_ROUND_EDGE_AND_DIM:
    std::invoke(PencilRenderCoreFunction::drawRoundEdgeAndDimRectangle, GameStruct::PencilRenderCore,
      position.x, position.y, target.x, target.y, RoundedEdgeType::SLIGHT);
    break;
  case RenderProviderHeader::STRONG_ROUND_EDGE_AND_DIM:
    std::invoke(PencilRenderCoreFunction::drawRoundEdgeAndDimRectangle, GameStruct::PencilRenderCore,
      position.x, position.y, target.x, target.y, RoundedEdgeType::STRONG);
    break;
  case RenderProviderHeader::SLIGHT_ROUND_EDGE_AND_COLOR:
    std::invoke(PencilRenderCoreFunction::drawRoundEdgeAndColorRectangle, GameStruct::PencilRenderCore,
      position.x, position.y, target.x, target.y, color, RoundedEdgeType::SLIGHT);
    break;
  case RenderProviderHeader::STRONG_ROUND_EDGE_AND_COLOR:
    std::invoke(PencilRenderCoreFunction::drawRoundEdgeAndColorRectangle, GameStruct::PencilRenderCore,
      position.x, position.y, target.x, target.y, color, RoundedEdgeType::STRONG);
    break;

  // needs special logic, since the code tries to put the rectangle inside the requested rect
  case RenderProviderHeader::BORDER_AND_DIM:
  case RenderProviderHeader::BORDER_AND_ALPHA_DIM:
    {
      const int width{ target.x - position.x + 1 };
      const int height{ target.y - position.y + 1 };
      const int widthReminder{ width % 24 };
      const int heightReminder{ height % 24 };
      const int sizeOfInsideWidth{ width - widthReminder + (widthReminder != 0) * 24 };
      const int sizeOfInsideHeight{ height - heightReminder + (heightReminder != 0) * 24 };
      const int adjustXPosition{ position.x - (24 + sizeOfInsideWidth - width) / 2 };
      const int adjustYPosition{ position.y - (24 + sizeOfInsideHeight - height) / 2 };

      if (type == RenderProviderHeader::BORDER_AND_DIM)
      {
        std::invoke(PencilRenderCoreFunction::drawBorderAndDimRectangle, GameStruct::PencilRenderCore,
          adjustXPosition, adjustYPosition, sizeOfInsideWidth + 1, sizeOfInsideHeight + 1);
      }
      else
      {
        std::invoke(PencilRenderCoreFunction::drawBorderAndAlphaDimRectangle, GameStruct::PencilRenderCore,
          adjustXPosition, adjustYPosition, sizeOfInsideWidth + 1, sizeOfInsideHeight + 1, blendStrength);
      }
    }
    break;

  default:
    Log(LogLevel::LOG_WARNING, "[RenderProvider]: Received unknown rectangle type for 'drawRectangle'. Ignoring request.");
    break;
  }
}
