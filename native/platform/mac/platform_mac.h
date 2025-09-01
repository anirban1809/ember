#pragma once
#ifdef __cplusplus
extern "C" {
#endif

// Accepts either NSWindow* or NSView* pointer (from Electron's
// getNativeWindowHandle)
void Mac_AttachGLViewToWindow(void *nsWindowOrView, int x, int y, int w, int h);
void Mac_ResizeGLView(int x, int y, int w, int h);
void Mac_SetIOSurfaceId(unsigned int ioSurfaceId);  // (stub for now)
void Mac_DestroyGLView(void);
void Mac_ButtonClick(void);

#ifdef __cplusplus
}
#endif
