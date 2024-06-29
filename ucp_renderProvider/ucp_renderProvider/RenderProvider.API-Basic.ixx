module;

#include "renderProviderHeader.h"

export module RenderProvider.API:Basic;

import RenderProvider.Logger;

import :RenderContext;
import :RenderState;

using namespace RenderProviderHeader;

export extern "C" __declspec(dllexport) FuncRequestRenderer requestRenderer;
export extern "C" __declspec(dllexport) FuncReleaseRenderer releaseRenderer;

export extern "C" __declspec(dllexport) FuncRender render;
