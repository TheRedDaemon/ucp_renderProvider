module;

#include "renderProviderHeader.h"

module RenderProvider.API:RenderContext;

import RenderProvider.Logger;

import :RenderContext;
using namespace RenderProviderHeader;

// general
extern "C" __declspec(dllexport) void __stdcall setRenderTarget(Renderer renderer, RenderTarget target)
{
  RenderContext::verifyAndExecute<&RenderContext::setTarget>(renderer, target);
}

extern "C" __declspec(dllexport) void __stdcall setRelativeRenderTargetRect(Renderer renderer, const Rect* rect)
{
  RenderContext::verifyAndExecute<&RenderContext::setRelativeRenderTargetRect>(renderer, rect);
}

extern "C" __declspec(dllexport) void __stdcall receiveScreenRect(Renderer renderer, Rect* rect)
{
  RenderContext::verifyAndExecute<&RenderContext::receiveScreenRect>(renderer, rect);
}

extern "C" __declspec(dllexport) void __stdcall receiveMenuRect(Renderer renderer, Rect* rect)
{
  RenderContext::verifyAndExecute<&RenderContext::receiveMenuRect>(renderer, rect);
}

extern "C" __declspec(dllexport) void __stdcall setPosition(Renderer renderer, const Coord position) {
  RenderContext::verifyAndExecute<&RenderContext::setPosition>(renderer, position);
}

extern "C" __declspec(dllexport) void __stdcall setTargetPosition(Renderer renderer, const Coord target)
{
  RenderContext::verifyAndExecute<&RenderContext::setTargetPosition>(renderer, target);
}

extern "C" __declspec(dllexport) void __stdcall setAlpha(Renderer renderer, float alpha)
{
  RenderContext::verifyAndExecute<&RenderContext::setAlpha>(renderer, alpha);
}


// text
extern "C" __declspec(dllexport) void __stdcall setFontSize(Renderer renderer, FontSize fontSize)
{
  RenderContext::verifyAndExecute<&RenderContext::setFontSize>(renderer, fontSize);
}

extern "C" __declspec(dllexport) void __stdcall setTextPrimaryColor(Renderer renderer, unsigned int color)
{
  RenderContext::verifyAndExecute<&RenderContext::setTextPrimaryColor>(renderer, color);
}

extern "C" __declspec(dllexport) void __stdcall setTextSecondaryColor(Renderer renderer, unsigned int color)
{
  RenderContext::verifyAndExecute<&RenderContext::setTextSecondaryColor>(renderer, color);
}

extern "C" __declspec(dllexport) void __stdcall setTextAlignment(Renderer renderer, TextAlignment alignment)
{
  RenderContext::verifyAndExecute<&RenderContext::setTextAlignment>(renderer, alignment);
}

extern "C" __declspec(dllexport) void __stdcall keepLeftAlignedTextPositionForNextText(Renderer renderer)
{
  RenderContext::verifyAndExecute<&RenderContext::keepLeftAlignedTextPositionForNextText>(renderer);
}

extern "C" __declspec(dllexport) void __stdcall setTextShadow(Renderer renderer, bool active)
{
  RenderContext::verifyAndExecute<&RenderContext::setTextShadow>(renderer, active);
}

extern "C" __declspec(dllexport) void __stdcall setTextMultiline(Renderer renderer, bool active)
{
  RenderContext::verifyAndExecute<&RenderContext::setTextMultiline>(renderer, active);
}

extern "C" __declspec(dllexport) void __stdcall setTextWidth(Renderer renderer, int width)
{
  RenderContext::verifyAndExecute<&RenderContext::setTextWidth>(renderer, width);
}


// pencil
extern "C" __declspec(dllexport) void __stdcall setPencilColor(Renderer renderer, unsigned int color)
{
  RenderContext::verifyAndExecute<&RenderContext::setPencilColor>(renderer, color);
}
