export module RenderProvider.SHC:WindowAndDirectDraw;

export namespace SHC
{
  /* Reduced to the likely relevant data. */
  struct WindowAndDirectDraw
  {
    const int unused[6];  // only here unused
    const int gameResolutionX;
    const int gameResolutionY;
    const int mainMenuBorderWidth;
    const int mainMenuBorderHeight;
    // here is more, but also not interesting here
  };
}