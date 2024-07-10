module;

#include <functional>

#include "renderProviderHeader.h"

module RenderProvider.API:RenderContext;

using namespace RenderProviderHeader;

template<auto functionPtr, typename... Args>
auto verifyAndExecute(Renderer renderer, Args&&... args)
{
  return std::invoke(functionPtr, RenderContext::verifyValidContext(renderer), std::forward<Args>(args)...);
}


// general
extern "C" __declspec(dllexport) void __stdcall setRenderTarget(Renderer renderer, RenderTarget target)
{
  verifyAndExecute<&RenderContext::setTarget>(renderer, target);
}

extern "C" __declspec(dllexport) void __stdcall setRelativeMenuTargetRect(Renderer renderer, const Rect* rect)
{
  if (!rect)
  {
    Log(LogLevel::LOG_ERROR, "[RenderProvider]: Received nullptr rect for 'setRelativeMenuTargetRect'. Ignoring request.");
    return;
  };
  verifyAndExecute<&RenderContext::setRelativeMenuTargetRect>(renderer, *rect);
}

extern "C" __declspec(dllexport) void __stdcall setRelativeGameTargetRect(Renderer renderer, const Rect* rect)
{
  if (!rect)
  {
    Log(LogLevel::LOG_ERROR, "[RenderProvider]: Received nullptr rect for 'setRelativeGameTargetRect'. Ignoring request.");
    return;
  };
  verifyAndExecute<&RenderContext::setRelativeGameTargetRect>(renderer, *rect);
}

extern "C" __declspec(dllexport) void __stdcall receiveScreenRect(Renderer renderer, Rect* rect)
{
  if (!rect)
  {
    Log(LogLevel::LOG_ERROR, "[RenderProvider]: Received nullptr rect for 'receiveScreenRect'. Ignoring request.");
    return;
  };
  verifyAndExecute<&RenderContext::receiveScreenRect>(renderer, *rect);
}

extern "C" __declspec(dllexport) void __stdcall receiveMenuRect(Renderer renderer, Rect* rect)
{
  if (!rect)
  {
    Log(LogLevel::LOG_ERROR, "[RenderProvider]: Received nullptr rect for 'receiveMenuRect'. Ignoring request.");
    return;
  };
  verifyAndExecute<&RenderContext::receiveMenuRect>(renderer, *rect);
}

extern "C" __declspec(dllexport) void __stdcall receiveMapRect(Renderer renderer, Rect* rect)
{
  if (!rect)
  {
    Log(LogLevel::LOG_ERROR, "[RenderProvider]: Received nullptr rect for 'receiveMapRect'. Ignoring request.");
    return;
  };
  verifyAndExecute<&RenderContext::receiveMapRect>(renderer, *rect);
}

extern "C" __declspec(dllexport) void __stdcall setAlpha(Renderer renderer, float alpha)
{
  verifyAndExecute<&RenderContext::setAlpha>(renderer, alpha);
}


// text
extern "C" __declspec(dllexport) void __stdcall setFontSize(Renderer renderer, FontSize fontSize)
{
  verifyAndExecute<&RenderContext::setFontSize>(renderer, fontSize);
}

extern "C" __declspec(dllexport) void __stdcall setTextPrimaryColor(Renderer renderer, unsigned int color)
{
  verifyAndExecute<&RenderContext::setTextPrimaryColor>(renderer, color);
}

extern "C" __declspec(dllexport) void __stdcall setTextSecondaryColor(Renderer renderer, unsigned int color)
{
  verifyAndExecute<&RenderContext::setTextSecondaryColor>(renderer, color);
}

extern "C" __declspec(dllexport) void __stdcall setTextAlignment(Renderer renderer, TextAlignment alignment)
{
  verifyAndExecute<&RenderContext::setTextAlignment>(renderer, alignment);
}

extern "C" __declspec(dllexport) void __stdcall resetTextPosition(Renderer renderer)
{
  verifyAndExecute<&RenderContext::requestTextPositionReset>(renderer);
}

extern "C" __declspec(dllexport) void __stdcall setTextShadow(Renderer renderer, bool active)
{
  verifyAndExecute<&RenderContext::setTextShadow>(renderer, active);
}

extern "C" __declspec(dllexport) void __stdcall setTextMultiline(Renderer renderer, bool active)
{
  verifyAndExecute<&RenderContext::setTextMultiline>(renderer, active);
}

extern "C" __declspec(dllexport) void __stdcall setTextWidth(Renderer renderer, int width)
{
  verifyAndExecute<&RenderContext::setTextWidth>(renderer, width);
}
