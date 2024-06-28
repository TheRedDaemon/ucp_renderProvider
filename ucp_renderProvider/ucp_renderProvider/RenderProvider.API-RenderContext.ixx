module;

#include <unordered_set>

export module RenderProvider.API:RenderContext;

export class RenderContext
{
private:
  std::unordered_set<RenderContext> existingContexts;

public:
  RenderContext createContext();

private:
  RenderContext();
  virtual ~RenderContext();

public:
  void initRender() const;
};