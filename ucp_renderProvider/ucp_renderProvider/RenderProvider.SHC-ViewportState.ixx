export module RenderProvider.SHC:ViewportState;

export namespace SHC
{
  struct ViewportState
  {
    const char ignored[0x80]; // currently not interesting
    const int currentCameraOffsetX;
    const int currentCameraOffsetY;
    // here is more, but not interesting at the moment
  };
}
