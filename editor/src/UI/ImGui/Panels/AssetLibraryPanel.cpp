#include "../../../../include/UI/ImGui/Panels/AssetLibraryPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include "Core/Types.h"
#include "ECS/Components/MaterialComponent.h"
#include <iostream>

GeneralDetailsPanel::GeneralDetailsPanel(ApplicationState& s,
                                         SceneManager& p_SceneManager)
    : state(s), m_SceneManager(p_SceneManager) {}

void GeneralDetailsPanel::Render() {
    SetPositionAndSize();

    ImGui::Begin("Assets");

    for (Entity e : m_SceneManager.GetActiveScene().View<MaterialComponent>()) {
        if (ImGui::TreeNode(m_SceneManager.GetEntityName(e.GetId()).c_str())) {
            ImGui::TreePop();
        }
        if (ImGui::IsItemClicked()) {
            m_SceneManager.SelectEntity(e.GetId());
        }
    }

    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::BeginMenu("Create")) {
            if (ImGui::MenuItem("Material")) {
                std::cout << "Create new material" << std::endl;
                Entity e = m_SceneManager.CreateEntity("Material");
                m_SceneManager.GetActiveScene().AddComponent<MaterialComponent>(
                    e, MaterialComponent());
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}
