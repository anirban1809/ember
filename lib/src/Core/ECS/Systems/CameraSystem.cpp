#include "ECS/Systems/CameraSystem.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "glm/glm.hpp"

void CameraSystem::UpdateView(Scene& scene) {
    for (auto e : scene.View<CameraComponent>()) {
        auto& cameraComponent = scene.GetComponent<CameraComponent>(e);

        cameraComponent.view = glm::lookAt(cameraComponent.cameraPosition,
                                           cameraComponent.cameraLook,
                                           glm::vec3(0.0f, 1.0f, 0.0f));
    }
}