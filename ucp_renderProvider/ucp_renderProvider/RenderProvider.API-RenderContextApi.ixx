module;

#include "renderProviderHeader.h"

export module RenderProvider.API:RenderContextApi;

using namespace RenderProviderHeader;

// general
export extern "C" __declspec(dllexport) Context::FuncSetRenderTarget setRenderTarget;
export extern "C" __declspec(dllexport) Context::FuncSetRelativeRenderTargetRect setRelativeRenderTargetRect;
export extern "C" __declspec(dllexport) Context::FuncReceiveScreenRect receiveScreenRect;
export extern "C" __declspec(dllexport) Context::FuncReceiveMenuRect receiveMenuRect;
export extern "C" __declspec(dllexport) Context::FuncSetPosition setPosition;
export extern "C" __declspec(dllexport) Context::FuncSetTargetPosition setTargetPosition;
export extern "C" __declspec(dllexport) Context::FuncSetAlpha setAlpha;

// text
export extern "C" __declspec(dllexport) Context::FuncSetFontSize setFontSize;
export extern "C" __declspec(dllexport) Context::FuncSetTextPrimaryColor setTextPrimaryColor;
export extern "C" __declspec(dllexport) Context::FuncSetTextSecondaryColor setTextSecondaryColor;
export extern "C" __declspec(dllexport) Context::FuncSetTextAlignment setTextAlignment;
export extern "C" __declspec(dllexport) Context::FuncKeepLeftAlignedTextPositionForNextText keepLeftAlignedTextPositionForNextText;
export extern "C" __declspec(dllexport) Context::FuncSetTextShadow setTextShadow;
export extern "C" __declspec(dllexport) Context::FuncSetTextMultiline setTextMultiline;
export extern "C" __declspec(dllexport) Context::FuncSetTextWidth setTextWidth;

// pencil
export extern "C" __declspec(dllexport) Context::FuncSetPencilColor setPencilColor;