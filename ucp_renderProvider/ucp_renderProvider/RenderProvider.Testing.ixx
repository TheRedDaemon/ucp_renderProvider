module;

#include <format>

#include "renderProviderHeader.h"

export module RenderProvider.Testing;

import RenderProvider.SHC;
import RenderProvider.API;
import RenderProvider.Logger;

export struct FakeTextureRenderCore
{
  using ActualMenuToMapSurface = void (FakeTextureRenderCore::*)();

  inline static ActualMenuToMapSurface actualMenuToMapSurface{ nullptr };

  // funcs

  void __thiscall detouredMenuToMapSurface();
};

RenderProviderHeader::FuncRenderAction testAction;

module :private;

void __stdcall testAction(RenderProviderHeader::RenderToken token, void* nothing)
{
  RenderProviderHeader::Renderer renderer{ receiveRenderer(token)};

  RenderProviderHeader::Rect rect;
  receiveMenuRect(renderer, &rect);
  setRelativeMenuTargetRect(renderer, &rect);

  setTextWidth(renderer, 10);
  setTextMultiline(renderer, false);
  setTextShadow(renderer, true);

  setTextSecondaryColor(renderer, 0xffffffff);

  setPosition(renderer, { rect.width, 0 });
  setTextPrimaryColor(renderer, 0x00ff0000);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::LEFT);
  renderText(token, "TesT");

  setPosition(renderer, { rect.width, 40 });
  setTextPrimaryColor(renderer, 0x0000ff00);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::CENTER);
  renderText(token, "TesT");

  setPosition(renderer, { rect.width, 80 });
  setTextPrimaryColor(renderer, 0x000000ff);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::RIGHT);
  renderText(token, "TesT");

  setTextMultiline(renderer, true);

  setPosition(renderer, { rect.width, 120 });
  setTextPrimaryColor(renderer, 0x00ff0000);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::LEFT);
  renderText(token, "TesT TesT");

  setPosition(renderer, { rect.width, 160 });
  setTextPrimaryColor(renderer, 0x0000ff00);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::CENTER);
  renderText(token, "TesT TesT");

  setPosition(renderer, { rect.width, 200 });
  setTextPrimaryColor(renderer, 0x000000ff);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::RIGHT);
  renderText(token, "TesT TesT");

  static bool fontTested = false;
  if (!fontTested)
  {
    const char* textString{ "Test, but a bit longer?" };
    Log(LogLevel::LOG_INFO, std::format("Test String: {}", textString).c_str());
    Log(LogLevel::LOG_INFO, std::format("Length Very Small: {}", computeTextWidth(RenderProviderHeader::FontSize::VERY_SMALL, textString)).c_str());
    Log(LogLevel::LOG_INFO, std::format("Length Small: {}", computeTextWidth(RenderProviderHeader::FontSize::SMALL, textString)).c_str());
    Log(LogLevel::LOG_INFO, std::format("Length Medium: {}", computeTextWidth(RenderProviderHeader::FontSize::MEDIUM, textString)).c_str());
    Log(LogLevel::LOG_INFO, std::format("Length Big: {}", computeTextWidth(RenderProviderHeader::FontSize::BIG, textString)).c_str());
    Log(LogLevel::LOG_INFO, std::format("Length Very Big: {}", computeTextWidth(RenderProviderHeader::FontSize::VERY_BIG, textString)).c_str());

    RenderProviderHeader::FontData fontData;
    receiveFontData(RenderProviderHeader::FontSize::VERY_SMALL, &fontData);
    Log(LogLevel::LOG_INFO, std::format("FontData Very Small:\n\tbaselineOffset: {}\n\tlineHeight: {}\n\tletterSpacing: {}\n\twhiteSpaceWidth: {}",
      fontData.baselineOffset, fontData.lineHeight, fontData.letterSpacing, fontData.whiteSpaceWidth).c_str());
    receiveFontData(RenderProviderHeader::FontSize::SMALL, &fontData);
    Log(LogLevel::LOG_INFO, std::format("FontData Small:\n\tbaselineOffset: {}\n\tlineHeight: {}\n\tletterSpacing: {}\n\twhiteSpaceWidth: {}",
      fontData.baselineOffset, fontData.lineHeight, fontData.letterSpacing, fontData.whiteSpaceWidth).c_str());
    receiveFontData(RenderProviderHeader::FontSize::MEDIUM, &fontData);
    Log(LogLevel::LOG_INFO, std::format("FontData Medium:\n\tbaselineOffset: {}\n\tlineHeight: {}\n\tletterSpacing: {}\n\twhiteSpaceWidth: {}",
      fontData.baselineOffset, fontData.lineHeight, fontData.letterSpacing, fontData.whiteSpaceWidth).c_str());
    receiveFontData(RenderProviderHeader::FontSize::BIG, &fontData);
    Log(LogLevel::LOG_INFO, std::format("FontData BIG:\n\tbaselineOffset: {}\n\tlineHeight: {}\n\tletterSpacing: {}\n\twhiteSpaceWidth: {}",
      fontData.baselineOffset, fontData.lineHeight, fontData.letterSpacing, fontData.whiteSpaceWidth).c_str());
    receiveFontData(RenderProviderHeader::FontSize::VERY_BIG, &fontData);
    Log(LogLevel::LOG_INFO, std::format("FontData Very Big:\n\tbaselineOffset: {}\n\tlineHeight: {}\n\tletterSpacing: {}\n\twhiteSpaceWidth: {}",
      fontData.baselineOffset, fontData.lineHeight, fontData.letterSpacing, fontData.whiteSpaceWidth).c_str());

    fontTested = true;
  }
}

void FakeTextureRenderCore::detouredMenuToMapSurface()
{
  // Testing:
  RenderProviderHeader::Renderer renderer{ requestRenderer() };

  setRenderTarget(renderer, RenderProviderHeader::RenderTarget::MENU);
  setAlpha(renderer, 1.f);
  setFontSize(renderer, RenderProviderHeader::FontSize::BIG);
  render(renderer, testAction, nullptr);
  releaseRenderer(renderer);
 

  // Run actual function
  (*this.*actualMenuToMapSurface)();
}