module;

#include <functional>

#include "renderProviderHeader.h"

module RenderProvider.API:TextureRenderCore;

import RenderProvider.GamePointer;
import RenderProvider.Logger;

import :RenderState;

using namespace RenderProviderHeader;

extern "C" __declspec(dllexport) unsigned short __stdcall transformBGRToDisplayFormat(unsigned int color)
{
  return std::invoke(TextureRenderCoreFunction::transform24bitBGRToDisplayFormat, GameStruct::TextureRenderCore, color);
}
