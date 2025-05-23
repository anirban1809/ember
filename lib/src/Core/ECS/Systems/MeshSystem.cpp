#include "ECS/Systems/MeshSystem.h"
#include "Core/ShaderProgram.h"
#include "Core/ShaderRegistry.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/MaterialComponent.h"
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

        if (e.HasComponent<TransformComponent>()) {
            TransformComponent& transform =
                e.GetComponent<TransformComponent>();
            glm::mat4 model = transform.GetTransform();
            shader->SetUniformMat4("model", model);
        }

        if (e.HasComponent<MaterialComponent>()) {
            auto& material = e.GetComponent<MaterialComponent>();

            shader->SetUniformFloat3("u_AlbedoColor", material.albedoColor);
            shader->SetUniformFloat("u_Metallic", material.metallic);
            shader->SetUniformFloat("u_Roughness", material.roughness);
            shader->SetUniformFloat("u_HeightScale", material.heightScale);

            uint32_t textureSlot = 0;
            // Bind Albedo Map
            if (material.textures.find("albedoMap") !=
                material.textures.end()) {
                material.textures["albedoMap"]->Bind(textureSlot);
                shader->SetUniformInt("u_albedoMap", textureSlot);
                textureSlot++;
            }

            // Bind Normal Map
            if (material.textures.find("normalMap") !=
                material.textures.end()) {
                material.textures["normalMap"]->Bind(textureSlot);
                shader->SetUniformInt("u_normalMap", textureSlot);
                textureSlot++;
            }

            // bind roughness map
            if (material.textures.find("roughnessMap") !=
                material.textures.end()) {
                material.textures["roughnessMap"]->Bind(textureSlot);
                shader->SetUniformInt("u_RoughnessMap", textureSlot);
                textureSlot++;
            }

            // bind heightmap
            if (material.textures.find("heightMap") !=
                material.textures.end()) {
                material.textures["heightMap"]->Bind(textureSlot);
                shader->SetUniformInt("u_HeightMap", textureSlot);
                textureSlot++;
            }
        }

        m_RenderContext->SubmitMesh(mesh);
    }
}