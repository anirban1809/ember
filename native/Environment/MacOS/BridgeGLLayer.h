#import <QuartzCore/QuartzCore.h>

typedef void (^DrawCallback)(int width, int height);

@interface BridgeGLLayer : CAOpenGLLayer
@end

BridgeGLLayer *CreateBridgeLayer(void);
void BridgeGLLayer_SetDrawCallback(DrawCallback cb);
