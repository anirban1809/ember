#ifndef __OPENGLRENDERCONTEXT_H__
#define __OPENGLRENDERCONTEXT_H__

#include "../../RenderContext.h"
#include "../../VertexArray.h"
#include "Core/ShaderProgram.h"
#include <memory>

class OpenGLRenderContext : public RenderContext {
   public:
    void BeginScene(std::vector<std::shared_ptr<ShaderProgram>>&) override;
    void SetGlobalShaders(
        const std::vector<std::shared_ptr<ShaderProgram>>&) override;
    void Submit(const std::vector<float>& vertices,
                const std::vector<uint32>& indices, const VertexLayout& layout,
                const glm::mat4& transform) override;
    void SubmitMesh(const Mesh& mesh) override;
    void SubmitArrays(std::shared_ptr<VertexArray> vao,
                      std::shared_ptr<ShaderProgram> shader,
                      uint32 vertexCount) override;

    void Submit(std::shared_ptr<VertexArray> vao,
                std::shared_ptr<ShaderProgram> shader) override;
    void EndScene() override;
    void Clear() override;

   private:
    std::vector<std::shared_ptr<VertexArray>> drawQueue;
    glm::mat4 viewProjection;
    std::vector<std::shared_ptr<ShaderProgram>> globalShaders;
};
#endif  // __OPENGLRENDERCONTEXT_H__