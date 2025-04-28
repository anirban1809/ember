#include "../../../../include/UI/ImGui/Panels/NodePropsPanel.h"
#include "../../../../include/UI/MacOS/OpenFilePanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/MaterialComponent.h"
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

        std::filesystem::path path(mesh.filePath);
        std::string fileName = path.filename().string();

        ImGui::Text("File: %s", fileName.c_str());

        if (ImGui::Button("Load")) {
            std::string selectedFilePath = OpenFileDialog(
                "/Users/anirban/Documents/Code/engine/editor/models");
            auto& meshComponent = selected.GetComponent<MeshComponent>();
            meshComponent.LoadMesh(selectedFilePath);
        }

        ImGui::NewLine();

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Add Material")) {
                selected.AddComponent<MaterialComponent>();
            }

            if (ImGui::MenuItem("Add SubMesh")) {
                std::cout << "Addind SubMesh" << std::endl;
            }
            ImGui::EndPopup();
        }
    }

    if (selected.HasComponent<TransformComponent>()) {
        auto& transform = selected.GetComponent<TransformComponent>();
        ImGui::Separator();
        ImGui::NewLine();
        float position = 0.0f;

        ImGui::Text("Position (X,Y,Z)");
        ImGui::DragFloat3("Position", glm::value_ptr(transform.position));

        ImGui::NewLine();
        ImGui::Text("Rotation (Pitch, Roll, Yaw)");
        ImGui::DragFloat3("Rotation", glm::value_ptr(transform.rotation));

        ImGui::NewLine();
        ImGui::Text("Scale (Width, Height, Depth)");

        ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale));
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

    if (selected.HasComponent<MaterialComponent>()) {
        auto& component = selected.GetComponent<MaterialComponent>();

        ImGui::Separator();
        ImGui::Text("Materials");

        ImGui::NewLine();

        ImGui::Text("Albedo Color");
        ImGui::ColorEdit3("Albedo", glm::value_ptr(component.albedoColor));

        ImGui::NewLine();
        ImGui::SliderFloat("Metallic", &component.metallic, 0.0f, 1.0f);

        ImGui::NewLine();
        ImGui::SliderFloat("Roughness", &component.roughness, 0.0f, 1.0f);
    }

    ImGui::End();
}