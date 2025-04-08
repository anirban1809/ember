#ifndef __RENDERCONTEXT_H__
#define __RENDERCONTEXT_H__

#include "ShaderProgram.h"
#include "VertexLayout.h"
#include "Mesh.h"

class RenderContext {
   public:
    static std::shared_ptr<RenderContext> Create();
    virtual ~RenderContext() = default;

    virtual void BeginScene(std::vector<std::shared_ptr<ShaderProgram>>&) = 0;

    virtual void Submit(const std::vector<float>& vertices,
                        const std::vector<uint32>& indices,
                        const VertexLayout& layout,
                        const glm::mat4& transform) = 0;

    virtual void Submit(const Mesh& mesh) = 0;

    virtual void EndScene() = 0;

    virtual void Clear() = 0;
};
#endif  // __RENDERCONTEXT_H__