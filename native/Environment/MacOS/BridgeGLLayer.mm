#import "BridgeGLLayer.h"
#import <OpenGL/gl3.h>
#import <OpenGL/OpenGL.h>
#include "../../renderer/gl_backend.h"

static DrawCallback gDrawCb = nil;

@implementation BridgeGLLayer

+ (void)load {
  NSLog(@"[BridgeGLLayer] Registered in ObjC runtime");
}

- (BOOL)isAsynchronous {
  return YES;
}

- (CGLPixelFormatObj)copyCGLPixelFormatForDisplayMask:(uint32_t)mask {
  (void)mask;
  CGLPixelFormatAttribute attrs[] = {
      kCGLPFAOpenGLProfile,
      (CGLPixelFormatAttribute)kCGLOGLPVersion_3_2_Core,
      kCGLPFAAccelerated,
      kCGLPFAColorSize,
      (CGLPixelFormatAttribute)24,
      kCGLPFAAlphaSize,
      (CGLPixelFormatAttribute)8,
      (CGLPixelFormatAttribute)0};
  CGLPixelFormatObj pf = NULL;
  GLint npix = 0;
  CGLChoosePixelFormat(attrs, &pf, &npix);
  return pf;
}

- (void)prepareOpenGL {
  // CAOpenGLLayer’s super doesn’t publicly expose prepareOpenGL; don’t call
  // super.
  glbackend::ensureInit();
}

- (void)drawInCGLContext:(CGLContextObj)ctx
             pixelFormat:(CGLPixelFormatObj)pf
            forLayerTime:(CFTimeInterval)t
             displayTime:(const CVTimeStamp *)ts {
  (void)pf;
  (void)t;
  (void)ts;

  // Size in *points*
  const CGSize boundsPt = self.bounds.size;

  // Convert to *device pixels*
  const CGFloat scale = (self.contentsScale > 0.0 ? self.contentsScale : 1.0);
  const int wPx = (int)llround(boundsPt.width * scale);
  const int hPx = (int)llround(boundsPt.height * scale);

  // Your GL code expects pixels; it will call glViewport(wPx, hPx)
  if (gDrawCb) {
    gDrawCb(wPx, hPx);
  } else {
    glbackend::draw(wPx, hPx);
  }

  glFlush();
  [super drawInCGLContext:ctx pixelFormat:pf forLayerTime:t displayTime:ts];
}

@end

BridgeGLLayer *CreateBridgeLayer(void) { return [BridgeGLLayer layer]; }
void BridgeGLLayer_SetDrawCallback(DrawCallback cb) { gDrawCb = [cb copy]; }
