#include "ECS/Systems/MeshSystem.h"
#include "Core/ShaderProgram.h"
#include "Core/ShaderRegistry.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Entity.h"
#include "ECS/Entity.inl"
#include "glm/glm.hpp"

void MeshSystem::Init(Scene& scene) {
    auto shader = ShaderRegistry::GetShader("main");
    CameraComponent* camera = nullptr;

    for (Entity e : scene.View<CameraComponent>()) {
        camera = &e.GetComponent<CameraComponent>();
        break;
    }

    shader->Bind();
    shader->SetUniformFloat3("diffuseColor",
                             glm::vec3(1.0f, 1.0f, 1.0f));  // example color
    shader->SetUniformMat4("view", camera->view);
    shader->SetUniformMat4("projection", camera->projection);
    shader->SetUniformFloat3("viewPos", camera->cameraPosition);
}

void MeshSystem::Render(Scene& scene) {
    auto shader = ShaderRegistry::GetShader("main");
    shader->Bind();
    for (Entity e : scene.View<MeshComponent>()) {
        auto& mesh = e.GetComponent<MeshComponent>();
        auto& transform = e.GetComponent<TransformComponent>();
        glm::mat4 model = transform.GetTransform();
        shader->SetUniformMat4("model", model);
        m_RenderContext->SubmitMesh(mesh);
    }
}