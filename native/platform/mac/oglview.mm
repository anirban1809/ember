// native/platform/mac/oglview.mm
#import <Cocoa/Cocoa.h>
#import "BridgeGLLayer.h"
#include "../../renderer/gl_backend.h"

// Keep references so we can resize/destroy later
static __weak NSView *gParentView = nil;
static BridgeGLLayer *gLayer = nil;

static inline void runOnMain(void (^block)(void)) {
  if ([NSThread isMainThread])
    block();
  else
    dispatch_async(dispatch_get_main_queue(), block);
}

// Attach GL sublayer to a *contentView* NSView pointer coming from Electron
extern "C" void Mac_AttachGLViewToWindow(const void *nsViewPtr, int x, int y,
                                         int w, int h) {
  runOnMain(^{
    NSView *parent = (__bridge NSView *)nsViewPtr;
    if (!parent) {
      NSLog(@"[oglview] parent view is nil");
      return;
    }

    NSLog(@"[oglview] Mac_AttachGLViewToWindow ptr=%p frame=%d,%d %dx%d",
          nsViewPtr, x, y, w, h);
    gParentView = parent;

    // Make sure the parent has a layer we can add sublayers to
    parent.wantsLayer = YES;
    if (!parent.layer)
      parent.layer = [CALayer layer];

    if (!gLayer) {
      // Provide the C++ draw callback (already uses GLSL 150)
      BridgeGLLayer_SetDrawCallback(
          [](int W, int H) { glbackend::draw(W, H); });
      gLayer = CreateBridgeLayer();
      gLayer.needsDisplayOnBoundsChange = YES;
      CGFloat scale = parent.window.screen
                          ? parent.window.screen.backingScaleFactor
                          : NSScreen.mainScreen.backingScaleFactor;
      gLayer.contentsScale = scale > 0 ? scale : 1.0;
      [parent.layer addSublayer:gLayer];
    }

    // Convert DOM (top-left) -> Cocoa (bottom-left) coordinates
    CGFloat H = parent.bounds.size.height;
    gLayer.frame = NSMakeRect((CGFloat)x, H - (CGFloat)y - (CGFloat)h,
                              (CGFloat)w, (CGFloat)h);
    [gLayer setNeedsDisplay];
  });
}

extern "C" void Mac_ResizeGLView(int x, int y, int w, int h) {
  runOnMain(^{
    if (!gLayer || !gParentView)
      return;
    CGFloat H = gParentView.bounds.size.height;
    gLayer.frame = NSMakeRect((CGFloat)x, H - (CGFloat)y - (CGFloat)h,
                              (CGFloat)w, (CGFloat)h);
    [gLayer setNeedsDisplay];
  });
}

extern "C" void Mac_DestroyGLView(void) {
  runOnMain(^{
    if (gLayer) {
      [gLayer removeFromSuperlayer];
      gLayer = nil;
    }
    gParentView = nil;
    glbackend::shutdown();
  });
}
