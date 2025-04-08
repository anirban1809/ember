#ifndef __OPENGLRENDERCONTEXT_H__
#define __OPENGLRENDERCONTEXT_H__

#include "../../RenderContext.h"
#include "../../VertexArray.h"

class OpenGLRenderContext : public RenderContext {
   public:
    void BeginScene(std::vector<std::shared_ptr<ShaderProgram>>&) override;
    void Submit(const std::vector<float>& vertices,
                const std::vector<uint32>& indices, const VertexLayout& layout,
                const glm::mat4& transform) override;
    void Submit(const Mesh& mesh) override;
    void EndScene() override;
    void Clear() override;

   private:
    std::vector<std::shared_ptr<VertexArray>> drawQueue;
    glm::mat4 viewProjection;
};
#endif  // __OPENGLRENDERCONTEXT_H__