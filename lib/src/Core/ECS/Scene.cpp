#include "ECS/Scene.h"
#include <forward_list>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
Scene::Scene(const std::string& name) { m_SceneName = name; }

/**
 * @brief Creates a new entity within the scene.
 *
 * This function generates a new entity with a unique identifier and associates
 * it with the current scene. The entity is added to the internal entity set and
 * its name is stored in the entity-name mapping.
 *
 * @param name A reference to a string representing the name of the entity.
 * @return Entity The newly created entity.
 */
Entity Scene::CreateEntity(std::string& name) {
    Entity e = Entity(entityId++, this);
    m_Entities.insert(e.GetId());
    m_EntityNames[e.GetId()] = name;
    return e;
}

void Scene::DestroyEntity(Entity entity) {
    uint32 id = entity.GetId();
    m_Entities.erase(entity.GetId());
    m_EntityNames.erase(entity.GetId());

    for (auto& [type, component] : components) {
        component.erase(entity.GetId());
    }
}

template <typename T, typename... Args>
T& Scene::AddComponent(Entity entity, Args&&... args) {
    std::type_index type = typeid(T);
    std::unordered_map<uint64, std::unique_ptr<Component>> component_map =
        components[type];

    component_map[entity.GetId()] =
        std::make_unique(std::forward<Args>(args)...);

    return component_map[entity.GetId()].get();
}

template <typename T>
T& Scene::GetComponent(Entity entity) {
    std::type_index type = typeid(T);
    auto component_map = components[type];
    component_map[entity.GetId()].get();
}

template <typename T>
bool Scene::HasComponent(Entity entity) const {
    std::type_index type = typeid(T);
    auto iter = components.find(type);

    if (iter == components.end()) {
        return false;
    }

    return iter->second.find(entity.GetId()) != iter->second.end();
}

template <typename T>
void Scene::RemoveComponent(Entity entity) {
    std::type_index type = typeid(T);
    components[type].erase(entity.GetId());
}

template <typename... Args>
std::vector<Entity> Scene::GetAllEntities() {
    std::vector<Entity> result;

    for (uint64 entity : m_Entities) {
        bool match = (HasComponent<Args>(Entity(entity, this)) && ...);
        if (match) {
            result.emplace_back(entity, this);
        }
    }

    return result;
}
