#include "../../../../include/UI/ImGui/Panels/ScenePropsPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include "Core/Light.h"
#include "Core/Texture.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/MaterialComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Entity.h"
#include "UI/SceneManager.h"
#include <iostream>
#include <Core/Utils/MeshImporter.h>
#include <vector>

ScenePropsPanel::ScenePropsPanel(ApplicationState& p_AppState,
                                 SceneManager& p_SceneManager)
    : m_AppState(p_AppState), m_SceneManager(p_SceneManager) {}

void ScenePropsPanel::Render() {
    SetPositionAndSize();
    ImGui::Begin("Scene Properties");

    // auto allEntities = m_SceneManager.GetAllEntities();

    std::vector<Entity> entities =
        m_SceneManager.GetActiveScene().View<MeshComponent, LightComponent>();

    for (Entity e :
         m_SceneManager.GetActiveScene()
             .Match<MeshComponent, LightComponent, CameraComponent>()) {
        if (ImGui::TreeNode(m_SceneManager.GetEntityName(e.GetId()).c_str())) {
            ImGui::TreePop();
        }
        if (ImGui::IsItemClicked()) {
            m_SceneManager.SelectEntity(e.GetId());
        }
    }

    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::BeginMenu("3D Object")) {
            if (ImGui::MenuItem("Object")) {
                Entity e = m_SceneManager.CreateEntity("E_StaticMesh");
                m_SceneManager.GetActiveScene().AddComponent<MeshComponent>(
                    e,
                    MeshComponent("/Users/anirban/Documents/Code/engine/editor/"
                                  "models/Cube.obj"));
                m_SceneManager.GetActiveScene()
                    .AddComponent<TransformComponent>(e, TransformComponent());
                // m_SceneManager.GetActiveScene().AddComponent<MaterialComponent>(
                //     e, MaterialComponent());

                // auto& mat =
                //     m_SceneManager.GetActiveScene().GetComponent<MaterialComponent>(
                //         e);
                // // mat.albedoColor = glm::vec3(1.0f, 0.5f, 0.31f);  //
                // Orange-ish mat.metallic = 0.1f; mat.roughness = 0.5f;

                // mat.textures["albedoMap"] = Texture::Create(
                //     "/Users/anirban/Documents/Code/engine/editor/textures/"
                //     "white_marble_03_2k_baseColor.tga",
                //     TextureType::Albedo, true);

                // mat.textures["normalMap"] = Texture::Create(
                //     "/Users/anirban/Documents/Code/engine/editor/textures/"
                //     "white_marble_03_2k_normal.tga",
                //     TextureType::Normal, false);

                // mat.textures["roughnessMap"] = Texture::Create(
                //     "/Users/anirban/Documents/Code/engine/editor/textures/"
                //     "white_marble_03_2k_roughness.tga",
                //     TextureType::Normal, false);

                // mat.textures["heightMap"] = Texture::Create(
                //     "/Users/anirban/Documents/Code/engine/editor/textures/"
                //     "white_marble_03_2k_height.tga",
                //     TextureType::Unknown, false);
            }

            if (ImGui::MenuItem("Cube")) {
            }
            if (ImGui::MenuItem("Sphere")) {
            }
            if (ImGui::MenuItem("Cone")) {
                Entity e = m_SceneManager.CreateEntity("E_StaticMesh");
                m_SceneManager.GetActiveScene().AddComponent<MeshComponent>(
                    e,
                    MeshComponent("/Users/anirban/Documents/Code/engine/editor/"
                                  "models/Cone.obj"));
                m_SceneManager.GetActiveScene()
                    .AddComponent<TransformComponent>(e, TransformComponent());
            }

            if (ImGui::MenuItem("Cylinder")) {
                Entity e = m_SceneManager.CreateEntity("E_StaticMesh");
                m_SceneManager.GetActiveScene().AddComponent<MeshComponent>(
                    e,
                    MeshComponent("/Users/anirban/Documents/Code/engine/editor/"
                                  "models/Cylinder.obj"));
                m_SceneManager.GetActiveScene()
                    .AddComponent<TransformComponent>(e, TransformComponent());
            }
            if (ImGui::MenuItem("Plane")) {
            }
            if (ImGui::MenuItem("Quad")) {
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Light")) {
            if (ImGui::MenuItem("Point")) {
                Entity e = m_SceneManager.CreateEntity("E_PointLight");
                LightComponent lightComponent = LightComponent();

                lightComponent.lightColor = glm::vec3(1.0f);
                lightComponent.lightPosition = glm::vec3(0.0f, 10.0f, 0.0f);

                m_SceneManager.GetActiveScene().AddComponent<LightComponent>(
                    e, lightComponent);
            }

            if (ImGui::MenuItem("Directional")) {
                std::cout << "Create Directional Light" << std::endl;
            }

            if (ImGui::MenuItem("Spot")) {
                std::cout << "Create Spot Light" << std::endl;
            }

            if (ImGui::MenuItem("Area")) {
                std::cout << "Create Area Light" << std::endl;
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}