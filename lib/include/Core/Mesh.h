#ifndef __MESH_H__
#define __MESH_H__

#include "Core/Material.h"
#include "Core/VertexArray.h"
#include "assimp/Importer.hpp"
#include <glm/glm.hpp>

struct Mesh {
    glm::mat4 transform;
    std::shared_ptr<Material> material;

    // GPU-side
    std::shared_ptr<VertexArray> vao;
    std::shared_ptr<VertexBuffer> vbo;
    std::shared_ptr<IndexBuffer> ibo;

    Mesh() = default;

    Mesh(const std::vector<float>& vertices, const std::vector<uint32>& indices,
         const VertexLayout& layout, const glm::mat4& transform,
         std::shared_ptr<Material> material)
        : transform(transform), material(material) {
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
#endif  // __MESH_H__