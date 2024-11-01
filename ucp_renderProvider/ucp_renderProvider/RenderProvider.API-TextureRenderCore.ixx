module;

#include "renderProviderHeader.h"

export module RenderProvider.API:TextureRenderCore;

using namespace RenderProviderHeader;

export extern "C" __declspec(dllexport) Misc::FuncTransformBGRToDisplayFormat transformBGRToDisplayFormat;
