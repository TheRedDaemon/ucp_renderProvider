module;

#include "renderProviderHeader.h"

export module RenderProvider.API:PencilRenderCore;

using namespace RenderProviderHeader;

export extern "C" __declspec(dllexport) Render::FuncDrawLine drawLine;
export extern "C" __declspec(dllexport) Render::FuncDrawRectangle drawRectangle;
