#include "UI/SceneManager.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/MeshComponent.h"
#include <iostream>
#include <vector>

Entity SceneManager::CreateEntity(const std::string &name) {
    return m_ActiveScene.CreateEntity(name);
}

void SceneManager::SelectEntity(uint64 entityId) {
    m_SelectedEntity = m_ActiveScene.GetEntityById(entityId);
}

void SceneManager::DestroyEntity(Entity entity) {
    m_ActiveScene.DestroyEntity(entity);
}

std::unordered_set<uint64> SceneManager::GetAllEntities() const {
    return m_ActiveScene.GetAllEntities();
}

std::string SceneManager::GetEntityName(uint64 id) const {
    return m_ActiveScene.m_EntityNames[id];
}

Entity SceneManager::GetSelectedEntity() { return m_SelectedEntity; }