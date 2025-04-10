#ifndef __RENDERCONTEXT_H__
#define __RENDERCONTEXT_H__

#include "ShaderProgram.h"
#include "VertexLayout.h"
#include "Mesh.h"
#include <memory>
#include <vector>

class RenderContext {
   public:
    /**
     * @brief
     *
     * @return std::shared_ptr<RenderContext>
     */
    static std::shared_ptr<RenderContext> Create();

    /**
     * @brief Destroy the Render Context object
     *
     */
    virtual ~RenderContext() = default;

    /**
     * @brief
     *
     */
    virtual void BeginScene(std::vector<std::shared_ptr<ShaderProgram>>&) = 0;

    /**
     * @brief Set the Global Shaders object
     *
     */
    virtual void SetGlobalShaders(
        const std::vector<std::shared_ptr<ShaderProgram>>&) = 0;

    virtual void Submit(const std::vector<float>& vertices,
                        const std::vector<uint32>& indices,
                        const VertexLayout& layout,
                        const glm::mat4& transform) = 0;

    virtual void SubmitMesh(const Mesh& mesh) = 0;
    virtual void SubmitArrays(std::shared_ptr<VertexArray> vao,
                              std::shared_ptr<ShaderProgram> shader,
                              uint32 vertexCount) = 0;
    virtual void Submit(std::shared_ptr<VertexArray> vao,
                        std::shared_ptr<ShaderProgram> shader) = 0;

    virtual void EndScene() = 0;

    virtual void Clear() = 0;
};
#endif  // __RENDERCONTEXT_H__