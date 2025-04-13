#include "ECS/Systems/GridSystem.h"
#include "Core/ShaderRegistry.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/GridComponent.h"
#include "ECS/Scene.h"
#include "glm/glm.hpp"

GridSystem::GridSystem(std::shared_ptr<RenderContext> context)
    : m_RenderContext(context) {
    InitFullscreenTriangle();
}

void GridSystem::InitFullscreenTriangle() {
    std::vector<float> vertices = {-1.0f, -1.0f, 3.0f, -1.0f, -1.0f, 3.0f};

    std::vector<uint32> indices = {0, 1, 2};

    VertexLayout layout = {
        {ShaderType::Float2, 0, 0}  // aPosition at location = 0
    };

    auto vbo = VertexBuffer::Create(
        vertices.data(), vertices.size() * sizeof(float), layout.GetStride());
    auto ibo = IndexBuffer::Create(indices.data(), indices.size());

    vao = VertexArray::Create();
    vao->AddVertexBuffer(vbo);
    vao->SetVertexLayout(layout);
    vao->SetIndexBuffer(ibo);
}

void GridSystem::Render(Scene& scene) {
    for (Entity e : scene.View<CameraComponent>()) {
        auto& component = e.GetComponent<CameraComponent>();

        glm::mat4 invView = glm::inverse(component.view);
        glm::mat4 invProjection = glm::inverse(component.projection);

        auto shader = ShaderRegistry::GetShader("grid");

        shader->Bind();
        shader->SetUniformMat4("invView", invView);
        shader->SetUniformMat4("invProjection", invProjection);
        shader->SetUniformFloat3("cameraPos", component.cameraPosition);
        shader->SetUniformFloat3("gridColor", glm::vec3(0.3f));

        m_RenderContext->SubmitArrays(
            vao, shader,
            3);  // non-indexed is also fine here, but we're using IBO
    }
}