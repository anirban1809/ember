#include "ECS/Systems/LightSystem.h"
#include "Core/ShaderProgram.h"
#include "Core/ShaderRegistry.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Entity.h"
#include "ECS/Entity.inl"
#include <memory>

void LightSystem::Update(Scene& scene) {
    std::shared_ptr<ShaderProgram> shader = ShaderRegistry::GetShader("main");

    for (Entity e : scene.View<LightComponent>()) {
        LightComponent& light = e.GetComponent<LightComponent>();
        shader->SetUniformFloat3("lightPos", light.lightPosition);
        shader->SetUniformFloat3("lightColor", light.lightColor);
    }
}