#ifndef RENDERER_H
#define RENDERER_H

#include "../../../vendor/glew-2.2.0/include/GL/glew.h"

enum class RendererAPI {
    None = 0,
    OpenGL = 1,
    DirectX = 2,
    Vulkan = 3,
    GMMX = 4
};

class Renderer {
   public:
    inline static RendererAPI GetRendererAPI() { return API; }

    Renderer();

    void Clear() const;

   private:
    static RendererAPI API;
};

#endif
