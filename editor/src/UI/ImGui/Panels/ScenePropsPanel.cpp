#include "../../../../include/UI/ImGui/Panels/ScenePropsPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "UI/SceneManager.h"
#include <iostream>
#include <Core/Utils/MeshImporter.h>

ScenePropsPanel::ScenePropsPanel(ApplicationState& p_AppState,
                                 SceneManager& p_SceneManager)
    : m_AppState(p_AppState), m_SceneManager(p_SceneManager) {}

void ScenePropsPanel::Render() {
    SetPositionAndSize();
    ImGui::Begin("Scene Properties");

    auto allEntities = m_SceneManager.GetAllEntities();

    for (uint64 e : allEntities) {
        if (ImGui::TreeNode(m_SceneManager.GetEntityName(e).c_str())) {
            ImGui::TreePop();
        }
        if (ImGui::IsItemClicked()) {
            m_SceneManager.SelectEntity(e);
        }
    }

    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::MenuItem("Add Static Mesh")) {
            Entity e = m_SceneManager.CreateEntity("E_StaticMesh");
            m_SceneManager.GetActiveScene().AddComponent<MeshComponent>(
                e, MeshComponent());
            m_SceneManager.GetActiveScene().AddComponent<TransformComponent>(
                e, TransformComponent());
        }
        if (ImGui::MenuItem("Add Light Object")) {
            Entity e = m_SceneManager.CreateEntity("E_PointLight");
            LightComponent lightComponent = LightComponent();

            lightComponent.lightColor = glm::vec3(1.0f);
            lightComponent.lightPosition = glm::vec3(0.0f, 10.0f, 0.0f);

            m_SceneManager.GetActiveScene().AddComponent<LightComponent>(
                e, lightComponent);
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}