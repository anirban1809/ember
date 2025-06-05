#ifndef __SCENE_H__
#define __SCENE_H__

#include "Core/Types.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Scene {
   public:
    Scene(const std::string& name = "Untitled Scene") { m_SceneName = name; }

    Entity CreateEntity(const std::string& name) {
        Entity e = Entity(entityId++, this);
        m_Entities.insert(e.GetId());
        m_EntityNames[e.GetId()] = name + std::to_string(e.GetId());
        m_EntityIdMap[e.GetId()] = e;
        return e;
    }

    Entity GetEntityById(uint64 id) { return m_EntityIdMap[id]; }

    void DestroyEntity(Entity entity) {
        uint32 id = entity.GetId();
        m_Entities.erase(entity.GetId());
        m_EntityNames.erase(entity.GetId());

        for (auto& [type, component] : components) {
            component.erase(entity.GetId());
        }
    }

    template <typename T, typename... Args>
    T& AddComponent(Entity entity, Args&&... args) {
        std::type_index type = typeid(T);
        std::unordered_map<uint64, std::shared_ptr<Component>>& component_map =
            components[type];

        component_map[entity.GetId()] =
            std::make_shared<T>(std::forward<Args>(args)...);

        return *static_cast<T*>(component_map[entity.GetId()].get());
    }

    template <typename T>
    T& AddComponent(Entity entity, std::shared_ptr<T> component) {
        std::type_index type = typeid(T);
        std::unordered_map<uint64, std::shared_ptr<Component>>& component_map =
            components[type];

        component_map[entity.GetId()] = component;

        return *static_cast<T*>(component_map[entity.GetId()].get());
    }

    template <typename T>
    T& GetComponent(Entity entity) {
        std::type_index type = std::type_index(typeid(T));
        auto poolIt = components.find(type);

        if (poolIt == components.end()) {
            throw std::runtime_error(
                "Component pool for requested type not found.");
        }

        auto& pool = poolIt->second;
        uint64 id = entity.GetId();

        auto compIt = pool.find(id);
        if (compIt == pool.end()) {
            throw std::runtime_error("Component not found for this entity.");
        }

        return *static_cast<T*>(compIt->second.get());
    }

    template <typename T>
    bool HasComponent(Entity entity) const {
        std::type_index type = typeid(T);
        auto iter = components.find(type);

        if (iter == components.end()) {
            return false;
        }

        return iter->second.find(entity.GetId()) != iter->second.end();
    }

    template <typename T>
    void RemoveComponent(Entity entity) {
        std::type_index type = typeid(T);
        components[type].erase(entity.GetId());
    }

    template <typename... Args>
    std::vector<Entity> View() {
        std::vector<Entity> result;

        for (uint64 entity : m_Entities) {
            bool match = (HasComponent<Args>(Entity(entity, this)) && ...);
            if (match) {
                result.emplace_back(entity, this);
            }
        }

        return result;
    }

    template <typename... Args>
    std::vector<Entity> Match() {
        std::vector<Entity> result;

        for (uint64 entity : m_Entities) {
            bool match = (HasComponent<Args>(Entity(entity, this)) || ...);
            if (match) {
                result.emplace_back(entity, this);
            }
        }

        return result;
    }

    // std::string GetEntityName(uint64 id) { return m_EntityNames[id]; }

    std::unordered_set<uint64> GetAllEntities() { return m_Entities; }
    std::unordered_map<uint64, Entity> m_EntityIdMap;
    std::unordered_map<uint64, std::string> m_EntityNames;

   private:
    uint64 entityId = 1;
    std::string m_SceneName;
    std::unordered_set<uint64> m_Entities;
    std::unordered_map<std::type_index,
                       std::unordered_map<uint64, std::shared_ptr<Component>>>
        components;
};

#endif  // __SCENE_H__