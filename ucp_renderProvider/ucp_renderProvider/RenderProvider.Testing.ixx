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
  setRelativeRenderTargetRect(renderer, &rect);
  const int relativeRightPosition = rect.right - rect.left;

  setTextWidth(renderer, 100);
  setTextMultiline(renderer, false);
  setTextShadow(renderer, true);

  setTextSecondaryColor(renderer, 0xffffffff);

  RenderProviderHeader::Coord position{ relativeRightPosition + 1, 0 };
  setPosition(renderer, position);
  setTextPrimaryColor(renderer, 0x00ff0000);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::LEFT);
  renderText(token, "TesT");
  position.x += receiveLastLeftAlignedTextXOffset(renderer);;
  setPosition(renderer, position);
  renderText(token, "2");

  setPosition(renderer, { relativeRightPosition, 40 });
  setTextPrimaryColor(renderer, 0x0000ff00);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::CENTER);
  renderText(token, "TesT");

  setPosition(renderer, { relativeRightPosition, 80 });
  setTextPrimaryColor(renderer, 0x000000ff);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::RIGHT);
  renderText(token, "TesT");

  setTextMultiline(renderer, true);

  position = { 0, 120 };
  setPosition(renderer, position);
  setTextPrimaryColor(renderer, 0x00ff0000);
  renderText(token, "This is a test");

  // NOTE: likely game bug: long word breaks do not count to Y Offset
  position.y += receiveLastMultilineTextYOffset(renderer);
  setPosition(renderer, position);
  setTextPrimaryColor(renderer, 0x0000ff00);
  renderText(token, "Thisisatestthisisatest");

  setTextShadow(renderer, false);

  position.y += receiveLastMultilineTextYOffset(renderer);
  setPosition(renderer, position);
  setTextPrimaryColor(renderer, 0x000000ff);
  renderText(token, "This is a test");

  setTextMultiline(renderer, false);

  setPosition(renderer, { relativeRightPosition + 1, 360 });
  setTextPrimaryColor(renderer, 0x00ff0000);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::LEFT);
  renderText(token, "TesT");

  setPosition(renderer, { relativeRightPosition, 400 });
  setTextPrimaryColor(renderer, 0x0000ff00);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::CENTER);
  renderText(token, "TesT");

  setFontSize(renderer, RenderProviderHeader::FontSize::SMALL);

  setPosition(renderer, { relativeRightPosition, 440 });
  setTextPrimaryColor(renderer, 0x000000ff);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::RIGHT);
  renderText(token, "TesTa");

  // TODO?: once the APIs are "all" done, consider if certain states even make sense, or if
  // it would be better to just provide it as part of the function call.
  // prime example would be the positions, which are known by the caller
  // consider!!

  setPosition(renderer, { 100, 100 });
  setTargetPosition(renderer, { 200, 200 });
  setPencilColor(renderer, 0x000000ff);
  drawLine(token);

  setPosition(renderer, { 150, 100 });
  setTargetPosition(renderer, { 150, 200 });
  setPencilColor(renderer, 0x0000ff00);
  drawLine(token);

  setPosition(renderer, { 100, 150 });
  setTargetPosition(renderer, { 200, 150 });
  setPencilColor(renderer, 0x00ff0000);
  drawLine(token);

  setPosition(renderer, { 200, 100 });
  setTargetPosition(renderer, { 100, 200 });
  setPencilColor(renderer, 0x00ffffff);
  drawLine(token);

  setPosition(renderer, { 500, 550 });
  setTargetPosition(renderer, { 400, 400 });
  setPencilColor(renderer, 0x00ff00ff);
  drawRectangle(token, false);

  setPosition(renderer, { 600, 500 });
  setTargetPosition(renderer, { 1550, 599 });
  setPencilColor(renderer, 0x0000ffff);
  drawRectangle(token, true);

  setRenderTarget(renderer, RenderProviderHeader::RenderTarget::GAME);

  setPosition(renderer, { 650, 0 });
  setTargetPosition(renderer, { 600, 599 });
  setPencilColor(renderer, 0x00ffffff);
  drawRectangle(token, true);

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