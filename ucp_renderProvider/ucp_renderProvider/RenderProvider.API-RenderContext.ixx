module;

#include <memory>
#include <unordered_set>

#include "renderProviderHeader.h"

export module RenderProvider.API:RenderContext;

import RenderProvider.Utility;
import RenderProvider.Logger;

using namespace RenderProviderHeader;

class RenderContext;

// source: https://www.cppstories.com/2021/heterogeneous-access-cpp20/#how-to-enable-it-for-unordered-containers
struct RenderContextHash
{
  using is_transparent = void;
  [[nodiscard]] size_t operator()(std::unique_ptr<RenderContext>& contextPtr) const
  {
    return std::hash<void*>{}(contextPtr.get());
  }
  [[nodiscard]] size_t operator()(const std::unique_ptr<RenderContext>& contextPtr) const
  {
    return std::hash<void*>{}(contextPtr.get());
  }
  [[nodiscard]] size_t operator()(RenderContext& contextPtr) const
  {
    return std::hash<void*>{}(&contextPtr);
  }
  [[nodiscard]] size_t operator()(const RenderContext& contextPtr) const
  {
    return std::hash<const void*>{}(&contextPtr);
  }
  [[nodiscard]] size_t operator()(Renderer renderer) const
  {
    return std::hash<void*>{}(reinterpret_cast<void*>(renderer));
  }
};

// source: https://stackoverflow.com/a/69496377
struct RenderContextEqual
{
  using is_transparent = void;
  bool operator()(const std::unique_ptr<RenderContext>& lhs, const std::unique_ptr<RenderContext>& rhs) const
  {
    return lhs == rhs;
  }
  bool operator()(const std::unique_ptr<RenderContext>& lhs, const RenderContext& rhs) const
  {
    return lhs.get() == &rhs;
  }
  bool operator()(const std::unique_ptr<RenderContext>& lhs, const Renderer& rhs) const
  {
    return lhs.get() == reinterpret_cast<void*>(rhs);
  }
  bool operator()(const RenderContext& lhs, const std::unique_ptr<RenderContext>& rhs) const
  {
    return &lhs == rhs.get();
  }
  bool operator()(const Renderer& lhs, const std::unique_ptr<RenderContext>& rhs) const
  {
    return reinterpret_cast<void*>(lhs) == rhs.get();
  }
};

export class RenderContext final
{
private:
  static inline std::unordered_set<std::unique_ptr<RenderContext>, RenderContextHash, RenderContextEqual> existingContexts{};

public:
  static RenderContext& createContext();
  static RenderContext& verifyValidContext(Renderer renderer);
  static bool removeContext(const RenderContext& context);
  static bool removeContext(Renderer renderer);

private:
  Utility::Rect relativeMenuRect;
  Utility::Rect relativeMapRect;
  int blendStrength;

  FontSize fontSize;
  TextAlignment textAlignment;
  int fontPrimaryColor;
  int fontSecondaryColor;

public:
  RenderContext();
  ~RenderContext();

  void setAlpha(float alpha); // transforms alpha to blendStrength
  int getBlendStrength() const;

  // TODO: implement, need to set game values
  void setRelativeMenuRect(Utility::Rect rect);
  Utility::Rect getRelativeMenuRect() const;
  void setRelativeMapRect(Utility::Rect rect);
  Utility::Rect getRelativeMapRect() const;

  void setFontSize(FontSize fontSize);
  FontSize getFontSize() const;
  void setFontPrimaryColor(int fontPrimaryColor);
  int getFontPrimaryColor() const;
  void setFontSecondaryColor(int fontSecondaryColor);
  int getFontSecondaryColor() const;
  void setFontAlignment(TextAlignment textAlignment);
  TextAlignment getFontAlignment() const;

  void initRender() const;
  const Renderer asRenderer() const;
};
