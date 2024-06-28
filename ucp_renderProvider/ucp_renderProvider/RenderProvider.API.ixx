module;

#include <functional>
#include <utility>
#include <memory>
#include <vector>

#include "renderProviderHeader.h"

export module RenderProvider.API;

export import :RenderContext;

import RenderProvider.Logger;
import RenderProvider.RenderState;
import RenderProvider.GamePointer;

using namespace RenderProviderHeader;

/* functions */

export extern "C" __declspec(dllexport) FuncRequestRenderToken requestRenderToken;
export extern "C" __declspec(dllexport) FuncReleaseRenderToken releaseRenderToken;

export extern "C" __declspec(dllexport) FuncRender render;


// TextManager
export extern "C" __declspec(dllexport) Render::FuncRenderGameInGameText renderGameInGameText;

export extern "C" __declspec(dllexport) Render::FuncComputeGameTextWidth computeGameTextWidth;
export extern "C" __declspec(dllexport) Render::FuncComputeTextWidth computeTextWidth;


/* static module global variables */

std::vector<std::unique_ptr<RenderState>> renderStates;

template<auto objectPtrPtr, auto functionPtrPtr, typename... Args>
auto verifyAndExecute(RenderToken token, Args&&... args)
{
  RenderStateKeeper::verifyActiveRenderer(token);
  return std::invoke(*functionPtrPtr, *objectPtrPtr, std::forward<Args>(args)...);
}

//extern "C" __declspec(dllexport) void __stdcall render(RPH::RenderTarget target, RPH::FuncRenderAction renderAction, void* misc)
//{
//  const RenderStateKeeper keeper;
//  renderAction(keeper.exposeAsConstInt(), misc);
//}
//
///* render functions */
//
//
///* text */
//
//extern "C" __declspec(dllexport) void __stdcall renderGameInGameText(RPH::RenderKey key, int textOffsetIndex, int textNumInGroup, int xParam, int yParam,
//  RPH::TextAlignment alignment, unsigned int color1, unsigned int color2, RPH::FontSize fontSize, RPH::TextXOffsetHandling keepOffsetX, int blendStrength)
//{
//  verifyAndExecute<&GameStruct::TextManager, &TextManagerDrawFunction::renderGameInGameText>(key, textOffsetIndex, textNumInGroup,
//    xParam, yParam, alignment, color1, color2, fontSize, keepOffsetX, blendStrength);
//};
//
//extern "C" __declspec(dllexport) int __stdcall computeGameTextWidth(RPH::RenderKey key, int textOffsetIndex,
//  int textNumInGroup, RPH::FontSize fontSize)
//{
//  return verifyAndExecute<&GameStruct::TextManager, &TextManagerDrawFunction::computeGameTextWidth>(key, textOffsetIndex, textNumInGroup, fontSize);
//}
//
//extern "C" __declspec(dllexport) int __stdcall computeTextWidth(RPH::RenderKey key, const char* text,
//  RPH::FontSize fontSize)
//{
//  return verifyAndExecute<&GameStruct::TextManager, &TextManagerDrawFunction::computeTextWidth>(key, text, fontSize);
//}