module;

#include <memory>
#include <unordered_set>
#include <functional>

#include "renderProviderHeader.h"

export module RenderProvider.API:RenderContext;

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
  static inline RenderContext* lastUsedContext{ nullptr };

  static bool isRectInsideBounds(const Rect& rect, const Rect& bounds);
  static void limitRectToBounds(Rect& rect, const Rect& bounds);

public:
  static RenderContext& createContext();
  static RenderContext& verifyValidContext(Renderer renderer);
  static bool removeContext(const RenderContext& context);
  static bool removeContext(Renderer renderer);

  template<auto functionPtr, typename... Args>
  static auto verifyAndExecute(Renderer renderer, Args&&... args)
  {
    return std::invoke(functionPtr, verifyValidContext(renderer), std::forward<Args>(args)...);
  }

private:
  bool active;

  RenderTarget target;
  Rect relativeRenderTargetRect;
  Coord position;
  Coord targetPosition;
  int blendStrength;

  FontSize fontSize;
  TextAlignment textAlignment;
  unsigned int textPrimaryColor;
  unsigned int textSecondaryColor;
  bool textShadow;
  bool textMultiline;
  int textWidth;

  unsigned short pencilColor;

public:
  RenderContext();
  ~RenderContext();

  void setTarget(RenderTarget target);
  void setRelativeRenderTargetRect(const Rect* rect);
  void receiveScreenRect(Rect* rectToFill);
  void receiveMenuRect(Rect* rectToFill);
  void receiveMapRect(Rect* rectToFill); // unused currently, since in-game map size handling is unknown at the moment
  void setPosition(const Coord position);
  Coord receiveAdjustedPosition() const;
  void setTargetPosition(const Coord position);
  Coord receiveAdjustedTargetPosition() const;
  void setAlpha(float alpha); // transforms alpha to blendStrength
  int getBlendStrength() const;


  void setFontSize(FontSize fontSize);
  FontSize getFontSize() const;
  void setTextPrimaryColor(unsigned int textPrimaryColor);
  unsigned int getTextPrimaryColor() const;
  void setTextSecondaryColor(unsigned int textSecondaryColor);
  unsigned int getTextSecondaryColor() const;
  void setTextAlignment(TextAlignment textAlignment);
  TextAlignment getTextAlignment() const;
  int receiveLastLeftAlignedTextXOffset() const;
  int receiveLastMultilineTextYOffset() const;
  void setTextShadow(bool textShadow);
  bool hasTextShadow() const;
  void setTextMultiline(bool textMultiline);
  bool isTextMultiline() const;
  void setTextWidth(int textWidth);
  int getTextWidth() const;

  void setPencilColor(unsigned int pencilColor);
  unsigned short getPencilColor() const;

  void setActive();
  void setInactive();
  const Renderer asRenderer() const;
};
