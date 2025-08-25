// native/oglview.mm
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION 1
#endif

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#import <IOSurface/IOSurface.h>
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>
#include <napi.h>

#include "tri_renderer.h" // <— our C++ triangle

// ---------- Globals ----------
static uint32_t gSurfaceID = 0;
static NSView *gHostView = nil;
static CAOpenGLLayer *gLayer = nil;
static dispatch_source_t gTimer = nil;

// Texture program (only used when an IOSurface ID is set)
static GLuint gTexProg = 0, gQuadVAO = 0, gQuadVBO = 0;

static const char *kVS_Tex = R"(#version 150
in vec2 aPos;
out vec2 vPos;
void main(){ vPos=aPos; gl_Position=vec4(aPos,0.0,1.0); }
)";

static const char *kFS_Tex = R"(#version 150
uniform sampler2DRect uTex;
uniform vec2 uSize;
in vec2 vPos; out vec4 oColor;
void main(){
  vec2 uv = (vPos*0.5 + 0.5) * uSize;
  oColor = texture(uTex, uv);
}
)";

static GLuint compile(GLenum type, const char *src) {
  GLuint s = glCreateShader(type);
  glShaderSource(s, 1, &src, nullptr);
  glCompileShader(s);
  GLint ok = 0;
  glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    GLchar log[4096];
    GLsizei n = 0;
    glGetShaderInfoLog(s, 4096, &n, log);
    fprintf(stderr, "[oglview] shader err:\n%s\n", log);
  }
  return s;
}

static void ensureTexProgram() {
  if (gTexProg)
    return;
  GLuint vs = compile(GL_VERTEX_SHADER, kVS_Tex);
  GLuint fs = compile(GL_FRAGMENT_SHADER, kFS_Tex);
  gTexProg = glCreateProgram();
  glAttachShader(gTexProg, vs);
  glAttachShader(gTexProg, fs);
  glLinkProgram(gTexProg);
  glDeleteShader(vs);
  glDeleteShader(fs);

  // Fullscreen quad
  const float verts[8] = {-1, -1, 1, -1, -1, 1, 1, 1};
  glGenVertexArrays(1, &gQuadVAO);
  glBindVertexArray(gQuadVAO);
  glGenBuffers(1, &gQuadVBO);
  glBindBuffer(GL_ARRAY_BUFFER, gQuadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  GLint loc = glGetAttribLocation(gTexProg, "aPos");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ---------- CAOpenGLLayer ----------
@interface IOSurfaceLayer : CAOpenGLLayer
@end

@implementation IOSurfaceLayer

// 1) Force a modern (3.2 Core) context so GLSL #version 150 works.
- (CGLPixelFormatObj)copyCGLPixelFormatForDisplayMask:(uint32_t)mask {
  CGLPixelFormatAttribute attrs[] = {
      kCGLPFAOpenGLProfile,
      (CGLPixelFormatAttribute)kCGLOGLPVersion_3_2_Core,
      kCGLPFAAccelerated,
      kCGLPFADoubleBuffer,
      kCGLPFAColorSize,
      (CGLPixelFormatAttribute)24,
      kCGLPFADepthSize,
      (CGLPixelFormatAttribute)0,
      (CGLPixelFormatAttribute)0};
  CGLPixelFormatObj pix = NULL;
  GLint npix = 0;
  CGLChoosePixelFormat(attrs, &pix, &npix);
  return pix; // CA retains a copy
}

- (instancetype)init {
  if ((self = [super init])) {
    self.asynchronous = YES;
    self.needsDisplayOnBoundsChange = YES;
    self.contentsScale = [NSScreen mainScreen].backingScaleFactor;
  }
  return self;
}

- (void)drawInCGLContext:(CGLContextObj)ctx
             pixelFormat:(CGLPixelFormatObj)pf
            forLayerTime:(CFTimeInterval)t
             displayTime:(const CVTimeStamp *)ts {
  // 2) Use pixel size for the viewport (bounds are in points).
  const CGFloat scale = self.contentsScale > 0
                            ? self.contentsScale
                            : [NSScreen mainScreen].backingScaleFactor;
  const GLint vpW = (GLint)lround(self.bounds.size.width * scale);
  const GLint vpH = (GLint)lround(self.bounds.size.height * scale);

  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, vpW, vpH);

  // Bright clear so we can SEE draw is happening
  glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  if (gSurfaceID != 0) {
    // --- IOSurface path ---
    IOSurfaceRef surf = IOSurfaceLookup(gSurfaceID);
    if (surf) {
      ensureTexProgram();
      GLuint tex = 0;
      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_RECTANGLE, tex);
      CGLTexImageIOSurface2D(ctx, GL_TEXTURE_RECTANGLE, GL_RGBA16F,
                             (GLsizei)IOSurfaceGetWidth(surf),
                             (GLsizei)IOSurfaceGetHeight(surf), GL_BGRA,
                             GL_HALF_FLOAT, surf, 0);
      glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glUseProgram(gTexProg);
      glUniform1i(glGetUniformLocation(gTexProg, "uTex"), 0);
      glUniform2f(glGetUniformLocation(gTexProg, "uSize"),
                  (GLfloat)IOSurfaceGetWidth(surf),
                  (GLfloat)IOSurfaceGetHeight(surf));
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_RECTANGLE, tex);
      glBindVertexArray(gQuadVAO);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      glBindVertexArray(0);
      glUseProgram(0);

      glDeleteTextures(1, &tex);
      CFRelease(surf);
    }
  } else {
    // --- Fallback: draw the blue triangle from the C++ helper ---
    tri::ensureInit();
    tri::drawBlueTriangle();
  }

  [super drawInCGLContext:ctx pixelFormat:pf forLayerTime:t displayTime:ts];
}

@end

// ---------- N-API + view plumbing ----------
static void ensureTick() {
  if (gTimer)
    return;
  gTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0,
                                  dispatch_get_main_queue());
  dispatch_source_set_timer(gTimer, DISPATCH_TIME_NOW,
                            (uint64_t)(NSEC_PER_SEC / 60),
                            (uint64_t)(NSEC_PER_MSEC));
  dispatch_source_set_event_handler(gTimer, ^{
    if (gLayer)
      [gLayer setNeedsDisplay];
  });
  dispatch_resume(gTimer);
}

Napi::Value AttachGLView(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() < 5)
    return env.Undefined();

  auto handleBuf = info[0].As<Napi::Buffer<uint8_t>>();
  const double x = info[1].ToNumber().DoubleValue();
  const double y = info[2].ToNumber().DoubleValue();
  const double w = info[3].ToNumber().DoubleValue();
  const double h = info[4].ToNumber().DoubleValue();

  void *raw = *reinterpret_cast<void *const *>(handleBuf.Data());
  NSView *content = (__bridge NSView *)raw;
  if (!content)
    return env.Undefined();

  dispatch_async(dispatch_get_main_queue(), ^{
    NSView *content = (__bridge NSView *)raw;
    if (!content)
      return;

    if (!gHostView) {
      gHostView = [[NSView alloc] initWithFrame:content.bounds];
      gHostView.wantsLayer = YES;

      // *** DEBUG: translucent overlay so you can SEE the native NSView area
      // ***
      gHostView.layer = [CALayer layer];
      gHostView.layer.backgroundColor =
          [[NSColor colorWithCalibratedRed:0.0 green:0.8 blue:0.4
                                     alpha:0.15] CGColor];
      gHostView.layer.zPosition = 99999; // be very sure we’re above WKWebView

      gLayer = [IOSurfaceLayer layer];
      gLayer.contentsScale = [NSScreen mainScreen].backingScaleFactor;
      gLayer.needsDisplayOnBoundsChange = YES;
      gLayer.asynchronous = YES;
      gLayer.frame = gHostView.bounds;
      gLayer.zPosition = 100000; // GL layer above the debug background
      [gHostView.layer addSublayer:gLayer];

      // *** CRUCIAL: put our host view ABOVE all siblings ***
      [content addSubview:gHostView positioned:NSWindowAbove relativeTo:nil];

      ensureTick();
    } else {
      // Always bring to front (WKWebView sometimes reinserts subviews)
      [gHostView removeFromSuperviewWithoutNeedingDisplay];
      [content addSubview:gHostView positioned:NSWindowAbove relativeTo:nil];
    }

    // CSS top-left -> Cocoa bottom-left
    NSRect b = [content bounds];
    CGFloat nx = (CGFloat)x, nh = (CGFloat)h;
    CGFloat ny = b.size.height - (CGFloat)y - nh;
    gHostView.frame = NSMakeRect(nx, ny, (CGFloat)w, nh);
    gLayer.frame = gHostView.bounds;

    [gHostView setNeedsDisplay:YES];
    [gLayer setNeedsDisplay];
  });

  return env.Undefined();
}

Napi::Value ResizeGLView(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() < 4 || !gHostView)
    return env.Undefined();

  const double x = info[0].ToNumber().DoubleValue();
  const double y = info[1].ToNumber().DoubleValue();
  const double w = info[2].ToNumber().DoubleValue();
  const double h = info[3].ToNumber().DoubleValue();

  dispatch_async(dispatch_get_main_queue(), ^{
    NSView *content = [gHostView superview];
    if (!content)
      return;

    // Keep it on top every time
    [gHostView removeFromSuperviewWithoutNeedingDisplay];
    [content addSubview:gHostView positioned:NSWindowAbove relativeTo:nil];

    NSRect b = [content bounds];
    CGFloat nx = (CGFloat)x, nh = (CGFloat)h;
    CGFloat ny = b.size.height - (CGFloat)y - nh;
    gHostView.frame = NSMakeRect(nx, ny, (CGFloat)w, nh);
    gLayer.frame = gHostView.bounds;

    [gHostView setNeedsDisplay:YES];
    [gLayer setNeedsDisplay];
  });

  return env.Undefined();
}

Napi::Value SetIOSurfaceId(const Napi::CallbackInfo &info) {
  if (info.Length() < 1)
    return info.Env().Undefined();
  gSurfaceID = info[0].ToNumber().Uint32Value();
  NSLog(@"[oglview] SetIOSurfaceId = %u", gSurfaceID);
  dispatch_async(dispatch_get_main_queue(), ^{
    if (gLayer)
      [gLayer setNeedsDisplay];
  });
  return info.Env().Undefined();
}

Napi::Value DestroyGLView(const Napi::CallbackInfo &info) {
  dispatch_async(dispatch_get_main_queue(), ^{
    if (gHostView) {
      [gHostView removeFromSuperview];
      gHostView = nil;
      gLayer = nil;
      tri::destroy();
      if (gQuadVBO) {
        glDeleteBuffers(1, &gQuadVBO);
        gQuadVBO = 0;
      }
      if (gQuadVAO) {
        glDeleteVertexArrays(1, &gQuadVAO);
        gQuadVAO = 0;
      }
      if (gTexProg) {
        glDeleteProgram(gTexProg);
        gTexProg = 0;
      }
    }
  });
  return info.Env().Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("attachGLView", Napi::Function::New(env, AttachGLView));
  exports.Set("resizeGLView", Napi::Function::New(env, ResizeGLView));
  exports.Set("setIOSurfaceId", Napi::Function::New(env, SetIOSurfaceId));
  exports.Set("destroyGLView", Napi::Function::New(env, DestroyGLView));
  return exports;
}

NODE_API_MODULE(oglview, Init)
