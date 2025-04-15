#include "../../../../include/UI/ImGui/Panels/NodePropsPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "UI/SceneManager.h"
#include "ECS/Entity.inl"
#include "glm/gtc/type_ptr.hpp"
#include <filesystem>
#include <iostream>

NodePropsPanel::NodePropsPanel(ApplicationState& m_AppState,
                               SceneManager& m_SceneManager)
    : state(m_AppState), m_SceneManager(m_SceneManager) {}

void NodePropsPanel::Render() {
    SetPositionAndSize();

    Entity selected = m_SceneManager.GetSelectedEntity();

    ImGui::Begin("Entity Properties");
    ImGui::Text("%s : %s", "Entity",
                m_SceneManager.GetActiveScene()
                    .m_EntityNames[selected.GetId()]
                    .c_str());
    ImGui::Separator();

    if (!selected.scene) {
        ImGui::End();
        return;
    }

    if (selected.HasComponent<MeshComponent>()) {
        auto& mesh =
            m_SceneManager.GetActiveScene().GetComponent<MeshComponent>(
                selected);

        auto& transform =
            m_SceneManager.GetActiveScene().GetComponent<TransformComponent>(
                selected);

        std::filesystem::path path(mesh.filePath);
        std::string fileName = path.filename().string();

        ImGui::Text("File: %s", fileName.c_str());
        ImGui::Button("Load");

        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();
        float position = 0.0f;

        ImGui::Text("Position");

        ImGui::DragFloat("Position X", &transform.position.x, 0.05f, -1000.0f,
                         1000.0f);
        ImGui::DragFloat("Position Y", &transform.position.y, 0.05f, -1000.0f,
                         1000.0f);
        ImGui::DragFloat("Position Z", &transform.position.z, 0.05f, -1000.0f,
                         1000.0f);

        ImGui::NewLine();
        ImGui::Text("Rotation");
        ImGui::DragFloat("Pitch", &transform.rotation.x, 0.05f, -1000.0f,
                         1000.0f);
        ImGui::DragFloat("Yaw", &transform.rotation.y, 0.05f, -1000.0f,
                         1000.0f);
        ImGui::DragFloat("Roll", &transform.rotation.z, 0.05f, -1000.0f,
                         1000.0f);

        ImGui::NewLine();
        ImGui::Text("Scale");
        ImGui::DragFloat("Width", &transform.scale.x, 0.05f, -1000.0f, 1000.0f);
        ImGui::DragFloat("Height", &transform.scale.y, 0.05f, -1000.0f,
                         1000.0f);
        ImGui::DragFloat("Depth", &transform.scale.z, 0.05f, -1000.0f, 1000.0f);
    }

    if (selected.HasComponent<LightComponent>()) {
        ImGui::NewLine();
        ImGui::Text("Light Position");
        auto& component =
            m_SceneManager.GetActiveScene().GetComponent<LightComponent>(
                selected);

        float x = component.lightPosition.x;
        float y = component.lightPosition.y;
        float z = component.lightPosition.z;

        ImGui::DragFloat("Position X", &component.lightPosition.x, 0.05f,
                         -100.0f, 100.0f);
        ImGui::DragFloat("Position Y", &component.lightPosition.y, 0.05f,
                         -100.0f, 100.0f);
        ImGui::DragFloat("Position Z", &component.lightPosition.z, 0.05f,
                         -100.0f, 100.0f);

        ImGui::NewLine();
        ImGui::Text("Light Color");
        ImGui::ColorEdit3("Light Color", glm::value_ptr(component.lightColor));
    }

    ImGui::End();
}