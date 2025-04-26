#ifndef __MESHCOMPONENT_H__
#define __MESHCOMPONENT_H__

#include "Core/Material.h"
#include "Core/Utils/MeshImporter.h"
#include "Core/VertexArray.h"
#include "Core/VertexLayout.h"
#include "ECS/Component.h"
#include "glm/glm.hpp"

struct MeshComponent : public Component {
    std::shared_ptr<VertexArray> vao;
    std::shared_ptr<VertexBuffer> vbo;
    std::shared_ptr<IndexBuffer> ibo;

    std::string filePath;

    MeshComponent() = default;

    MeshComponent(const std::string& path) { LoadMesh(path); }

    void LoadMesh(const std::string& path) {
        filePath = path;

        VertexLayout layout = VertexLayout{
            {ShaderType::Float3, 0 * sizeof(float), 0},  // position
            {ShaderType::Float2, 3 * sizeof(float), 1},  // texcoord
            {ShaderType::Float3, 5 * sizeof(float), 2},  // normal
            {ShaderType::Float3, 8 * sizeof(float), 3},  // tangent
            {ShaderType::Float3, 11 * sizeof(float), 4}  // bitangent
        };

        std::vector<float> vertices;
        std::vector<uint32> indices;

        Importer::LoadFile(path, vertices, indices);

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