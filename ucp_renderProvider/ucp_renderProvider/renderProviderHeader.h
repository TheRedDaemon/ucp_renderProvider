
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

  enum LeftAlignedTextXOffsetHandling : int
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
    VERY_BIG = 15,
    BIG = 16,
    MEDIUM = 17,
    SMALL = 18,
    VERY_SMALL = 19,
  };

  struct FontData
  {
    int baselineOffset;
    int lineHeight;
    int letterSpacing;
    int whiteSpaceWidth;
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

  struct Coord
  {
    int x;
    int y;
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
    using FuncSetPosition = void(__stdcall)(Renderer renderer, const Coord position);
    using FuncSetTargetPosition = void(__stdcall)(Renderer renderer, const Coord target);
    using FuncSetAlpha = void(__stdcall)(Renderer renderer, float alpha);

    // text
    using FuncSetFontSize = void(__stdcall)(Renderer renderer, FontSize fontSize);
    using FuncSetTextPrimaryColor = void(__stdcall)(Renderer renderer, unsigned int color); // color: 0x00BBGGRR
    using FuncSetTextSecondaryColor = void(__stdcall)(Renderer renderer, unsigned int color); // color: 0x00BBGGRR
    using FuncSetTextAlignment = void(__stdcall)(Renderer renderer, TextAlignment alignment);
    using FuncKeepLeftAlignedTextPositionForNextText = void(__stdcall)(Renderer renderer); // only works for single line left alignment
    using FuncSetTextShadow = void(__stdcall)(Renderer renderer, bool active);
    using FuncSetTextMultiline = void(__stdcall)(Renderer renderer, bool active); // multiline is always left aligned
    using FuncSetTextWidth = void(__stdcall)(Renderer renderer, int width);

    // pencil
    using FuncSetPencilColor = void(__stdcall)(Renderer renderer, unsigned int color);
  }

  namespace Render
  {
    using FuncReceiveRenderer = Renderer(__stdcall)(RenderToken token);

    // text
    using FuncRenderText = void(__stdcall)(RenderToken token, const char* text);

    // pencil
    using FuncDrawLine = void(__stdcall)(RenderToken token);
    using FuncDrawRectangle = void(__stdcall)(RenderToken token, bool fill);
  };

  namespace Misc
  {
    using FuncReceiveFontData = void(__stdcall)(RenderProviderHeader::FontSize fontSize, RenderProviderHeader::FontData* receiver);
    using FuncComputeTextWidth = int(__stdcall)(RenderProviderHeader::FontSize fontSize, const char* text);
    using FuncTransformBGRToDisplayFormat = unsigned short(__stdcall)(unsigned int color);
  }

  // Cpp API
  //using FuncTextReceiver = void(__stdcall*)(const char* transformedString, void* misc);
  //inline constexpr char const* NAME_SET_TEXT{ "_SetText@12" };
  //inline FuncSetText SetText{ nullptr };
}

#endif // RENDER_PROVIDER_HEADER
