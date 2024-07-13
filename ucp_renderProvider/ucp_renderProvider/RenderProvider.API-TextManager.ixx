module;

#include "renderProviderHeader.h"

export module RenderProvider.API:TextManager;

using namespace RenderProviderHeader;

export extern "C" __declspec(dllexport) Render::FuncRenderText renderText;

export extern "C" __declspec(dllexport) Misc::FuncReceiveFontData receiveFontData;
export extern "C" __declspec(dllexport) Misc::FuncComputeTextWidth computeTextWidth;
