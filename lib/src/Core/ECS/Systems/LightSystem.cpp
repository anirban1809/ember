#include "ECS/Systems/LightSystem.h"
#include "Core/ShaderProgram.h"
#include "Core/ShaderRegistry.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Entity.h"
#include "ECS/Entity.inl"
#include <memory>

void LightSystem::Update(Scene& scene) {
    std::shared_ptr<ShaderProgram> shader = ShaderRegistry::GetShader("main");

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

    for (Entity e : scene.View<LightComponent>()) {
        LightComponent& light = e.GetComponent<LightComponent>();
        lightPositions.push_back(light.lightPosition);
        lightColors.push_back(light.lightColor);
    }

    // Upload light count
    shader->SetUniformInt("lightCount",
                          static_cast<int>(lightPositions.size()));

    // Upload light data as array
    for (size_t i = 0; i < lightPositions.size(); ++i) {
        shader->SetUniformFloat3("lights[" + std::to_string(i) + "].position",
                                 lightPositions[i]);
        shader->SetUniformFloat3("lights[" + std::to_string(i) + "].color",
                                 lightColors[i]);
    }
}