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
  const int relativeRightPosition = rect.limits.right - rect.limits.left;

  setTextWidth(renderer, 100);
  setTextMultiline(renderer, false);
  setTextShadow(renderer, true);

  setTextSecondaryColor(renderer, 0xffffffff);

  RenderProviderHeader::Coord position{ 0, 0 };

  setTextPrimaryColor(renderer, 0x00ff0000);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::LEFT);
  position.x = relativeRightPosition + 1;
  renderText(token, "TesT", &position);
  position.x += receiveLastLeftAlignedTextXOffset(renderer);
  renderText(token, "2", &position);

  setTextPrimaryColor(renderer, 0x0000ff00);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::CENTER);
  position = { relativeRightPosition, 40 };
  renderText(token, "TesT", &position);

  setTextPrimaryColor(renderer, 0x000000ff);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::RIGHT);
  position = { relativeRightPosition, 80 };
  renderText(token, "TesT", &position);

  setTextMultiline(renderer, true);

  position = { 0, 120 };
  setTextPrimaryColor(renderer, 0x00ff0000);
  renderText(token, "This is a test", &position);

  // NOTE: likely game bug: long word breaks do not count to Y Offset
  position.y += receiveLastMultilineTextYOffset(renderer);
  setTextPrimaryColor(renderer, 0x0000ff00);
  renderText(token, "Thisisatestthisisatest", &position);

  setTextShadow(renderer, false);

  position.y += receiveLastMultilineTextYOffset(renderer);
  setTextPrimaryColor(renderer, 0x000000ff);
  renderText(token, "This is a test", &position);

  setTextMultiline(renderer, false);

  setTextPrimaryColor(renderer, 0x00ff0000);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::LEFT);
  position = { relativeRightPosition + 1, 360 };
  renderText(token, "TesT", &position);

  setTextPrimaryColor(renderer, 0x0000ff00);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::CENTER);
  position = { relativeRightPosition, 400 };
  renderText(token, "TesT", &position);

  setFontSize(renderer, RenderProviderHeader::FontSize::SMALL);

  setTextPrimaryColor(renderer, 0x000000ff);
  setTextAlignment(renderer, RenderProviderHeader::TextAlignment::RIGHT);
  position = { relativeRightPosition, 440 };
  renderText(token, "TesTa", &position);

  // TODO?: once the APIs are "all" done, consider if certain states even make sense, or if
  // it would be better to just provide it as part of the function call

  RenderProviderHeader::Rect positionRect{};

  positionRect = { 100, 100, 200, 200 };
  setPencilColor(renderer, 0x000000ff);
  drawLine(token, &positionRect);

  positionRect = { 150, 100, 150, 200 };
  setPencilColor(renderer, 0x0000ff00);
  drawLine(token, &positionRect);

  positionRect = { 100, 150, 200, 150 };
  setPencilColor(renderer, 0x00ff0000);
  drawLine(token, &positionRect);

  positionRect = { 200, 100, 100, 200 };
  setPencilColor(renderer, 0x00ffffff);
  drawLine(token, &positionRect);

  positionRect = { 500, 550, 400, 400 };
  setPencilColor(renderer, 0x00ff00ff);
  drawRectangle(token, RenderProviderHeader::RectangleType::BORDER, &positionRect);

  positionRect = { 600, 500, 1550, 599 };
  setPencilColor(renderer, 0x0000ffff);
  drawRectangle(token, RenderProviderHeader::RectangleType::COLOR, &positionRect);

  setRenderTarget(renderer, RenderProviderHeader::RenderTarget::GAME);

  positionRect = { 650, 0, 600, 599 };
  setPencilColor(renderer, 0x00ffffff);
  drawRectangle(token, RenderProviderHeader::RectangleType::COLOR, &positionRect);

  setRenderTarget(renderer, RenderProviderHeader::RenderTarget::MENU);

  positionRect = { 600, 300, 649, 349 };
  drawRectangle(token, RenderProviderHeader::RectangleType::DIM, &positionRect);

  setAlpha(renderer, 0.3f);
  positionRect = { 650, 300, 699, 349 };
  drawRectangle(token, RenderProviderHeader::RectangleType::ALPHA_BLEND, &positionRect);

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