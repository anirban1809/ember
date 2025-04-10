#ifndef __SKYBOXRENDERER_H__
#define __SKYBOXRENDERER_H__

#include "Core/Camera.h"
#include "Core/FrameBuffer.h"
#include "Core/RenderContext.h"
#include "Core/VertexArray.h"
#include "UI/TextureCube.h"
#include <memory>
class SkyboxRenderer {
   public:
    SkyboxRenderer();

    void Render(std::shared_ptr<RenderContext> context,
                std::shared_ptr<ShaderProgram> shader,
                std::shared_ptr<TextureCube> cubemap, Camera& camera);

   private:
    void InitCube();
    std::shared_ptr<VertexArray> vao;
    std::shared_ptr<FrameBuffer> framebuffer;
    uint32 indexCount = 0;
};

#endif  // __SKYBOXRENDERER_H__