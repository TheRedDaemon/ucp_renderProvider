
#ifndef RENDER_PROVIDER_HEADER
#define RENDER_PROVIDER_HEADER

#include <lua.hpp>

namespace RenderProviderHeader
{
  inline constexpr char const* NAME_MODULE{ "renderProvider" };
  inline constexpr char const* NAME_VERSION{ "0.0.1" };

  /* Enums */

  enum class RenderTarget
  {
    MENU = 0,
    GAME = 1,
  };

  /* Functions */

  using Renderer = void*;

  using FuncRenderAction = void(__stdcall*)(Renderer renderer, void* misc);
  using FuncRender = void(__stdcall*)(const RenderTarget target, FuncRenderAction renderAction, void* misc);
  using FuncSizedRender = void(__stdcall*)(const RenderTarget target, int xStart, int xEnd, int yStart, int yEnd,
    FuncRenderAction renderAction, void* misc);

  namespace Render
  {
    // will contains functions as stdcalls
    // using Func... = ...(__stdcall*)(Renderer renderer, ...)
  };

  // Cpp API
  //using FuncTextReceiver = void(__stdcall*)(const char* transformedString, void* misc);
  //inline constexpr char const* NAME_SET_TEXT{ "_SetText@12" };
  //inline FuncSetText SetText{ nullptr };
}

#endif // RENDER_PROVIDER_HEADER
