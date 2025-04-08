#ifndef __RENDERAPI_H__
#define __RENDERAPI_H__

enum class GraphicsAPI { None, OpenGL, Vulkan };

class RenderAPI {
   public:
    static void Set(GraphicsAPI api);
    static GraphicsAPI Get();

   private:
    static GraphicsAPI current;
};

#endif  // __RENDERAPI_H__