export module RenderProvider.SHC:RenderTarget;

export namespace SHC
{
  /* The enum describes the rendering target. */
  enum class RenderTarget
  { 
    SCREEN_MENU = 0,
    MAP_GAME = 1,
    BUTTON_AND_ALPHA = 2, // Memory used for alpha textures for example.
    CONTEXT_BASED = -1 // Used to indicate to some functions, that a surface should be choosen based on context.
  };
}