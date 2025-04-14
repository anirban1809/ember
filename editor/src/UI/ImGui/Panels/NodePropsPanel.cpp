#include "../../../../include/UI/ImGui/Panels/NodePropsPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "UI/SceneManager.h"
#include "ECS/Entity.inl"
#include "glm/gtc/type_ptr.hpp"
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