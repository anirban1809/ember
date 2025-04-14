#ifndef __MESHCOMPONENT_H__
#define __MESHCOMPONENT_H__

#include "Core/Material.h"
#include "Core/VertexArray.h"
#include "ECS/Component.h"
#include "glm/glm.hpp"

struct MeshComponent : public Component {
    std::shared_ptr<VertexArray> vao;
    std::shared_ptr<VertexBuffer> vbo;
    std::shared_ptr<IndexBuffer> ibo;

    MeshComponent() = default;

    MeshComponent(const std::vector<float>& vertices,
                  const std::vector<uint32>& indices,
                  const VertexLayout& layout) {
        vao = VertexArray::Create();
        vbo = VertexBuffer::Create(vertices.data(),
                                   vertices.size() * sizeof(float),
                                   layout.GetStride());
        ibo = IndexBuffer::Create(indices.data(), indices.size());
        vao->AddVertexBuffer(vbo);
        vao->SetVertexLayout(layout);
        vao->SetIndexBuffer(ibo);
    }
};
#endif  // __MESHCOMPONENT_H__