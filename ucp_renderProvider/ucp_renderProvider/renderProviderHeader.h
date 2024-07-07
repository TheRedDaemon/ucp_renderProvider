
#ifndef RENDER_PROVIDER_HEADER
#define RENDER_PROVIDER_HEADER

// TODO?

#include <lua.hpp>
#include <ucp3.h>

namespace RenderProviderHeader
{
  inline constexpr char const* NAME_MODULE{ "renderProvider" };
  inline constexpr char const* NAME_VERSION{ "0.0.1" };

  /* Enums */

  /* The enum describes the rendering target. */
  enum class RenderTarget
  {
    MENU = 0,
    GAME = 1,
    BUTTON_AND_ALPHA = 2, // internal, do not use, Memory used for alpha textures for example.
    CONTEXT_BASED = -1 // internal, do not use, used to indicate to some functions that a surface should be chosen based on context.
  };

  enum TextXOffsetHandling : int
  {
    DISCARD = 0,
    KEEP = 1,
  };

  enum TextAlignment : int
  {
    LEFT = 0,
    CENTER = 1,
    RIGHT = -1,
  };

  enum FontSize : int
  {
  };

  /* Structs */

  struct Range
  {
    int start;
    int end;
  };

  struct Rect
  {
    int x;
    int y;
    int width;
    int height;
  };

  /* Functions */

  using Renderer = const int; // configure a renderer state
  using RenderToken = const int; // requests actual render actions with the currently active renderer

  using FuncRequestRenderer = Renderer(__stdcall)();
  using FuncReleaseRenderer = void(__stdcall)(Renderer renderer);

  using FuncRenderAction = void(__stdcall)(RenderToken token, void* misc);
  using FuncRender = void(__stdcall)(Renderer renderer, FuncRenderAction renderAction, void* misc);

  namespace Context
  {
    // general
    using FuncSetRenderTarget = void(__stdcall)(Renderer renderer, RenderTarget target);
    using FuncSetRelativeMenuTargetRect = void(__stdcall)(Renderer renderer, const Rect* rect);
    using FuncSetRelativeGameTargetRect = void(__stdcall)(Renderer renderer, const Rect* rect);
    using FuncReceiveScreenRect = void(__stdcall)(Renderer renderer, Rect* rect);
    using FuncReceiveMenuRect = void(__stdcall)(Renderer renderer, Rect* rect);
    using FuncReceiveMapRect = void(__stdcall)(Renderer renderer, Rect* rect);
    using FuncSetAlpha = void(__stdcall)(Renderer renderer, float alpha);

    // text
    using FuncSetFontSize = void(__stdcall)(Renderer renderer, int fontSize);
    using FuncSetFontPrimaryColor = void(__stdcall)(Renderer renderer, unsigned int color);
    using FuncSetFontSecondaryColor = void(__stdcall)(Renderer renderer, unsigned int color);
    using FuncSetTextAlignment = void(__stdcall)(Renderer renderer, TextAlignment alignment);
    using FuncResetTextPosition = void(__stdcall)(Renderer renderer);
    using FuncSetTextWidth = void(__stdcall)(Renderer renderer, int width);
  }

  namespace Render
  {
    using FuncReceiveRenderer = Renderer(__stdcall)(RenderToken token);

    // will contains functions as stdcalls
    // using Func... = ...(__stdcall*)(RenderKey key, ...)

    /* Text */

    using FuncRenderGameInGameText = void(__stdcall)(RenderToken token, int textOffsetIndex, int textNumInGroup, int xParam, int yParam,
      TextAlignment alignment, unsigned int color1, unsigned int color2, FontSize fontSize, TextXOffsetHandling keepOffsetX, int blendStrength);

    // utility
    using FuncComputeGameTextWidth = int(__stdcall)(RenderToken token, int textOffsetIndex, int textNumInGroup, RenderProviderHeader::FontSize fontSize);
    using FuncComputeTextWidth = int(__stdcall)(RenderToken token, const char* text, RenderProviderHeader::FontSize fontSize);
  };

  // Cpp API
  //using FuncTextReceiver = void(__stdcall*)(const char* transformedString, void* misc);
  //inline constexpr char const* NAME_SET_TEXT{ "_SetText@12" };
  //inline FuncSetText SetText{ nullptr };
}

#endif // RENDER_PROVIDER_HEADER
