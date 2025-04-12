#ifndef __SCENE_H__
#define __SCENE_H__

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
class Scene {
   public:
    Scene(const std::string& name = "Untitled Scene");

    Entity CreateEntity(std::string&);

    void DestroyEntity(Entity);

    template <typename T, typename... Args>
    T& AddComponent(Entity entity, Args&&... args);

    template <typename T>
    T& GetComponent(Entity entity);

    template <typename T>
    bool HasComponent(Entity entity) const;

    template <typename T>
    void RemoveComponent(Entity entity);

    template <typename... Args>
    std::vector<Entity> GetAllEntities();

   private:
    uint64 entityId = 1;
    std::string m_SceneName;
    std::unordered_set<uint64> m_Entities;
    std::unordered_map<uint64, std::string> m_EntityNames;
    std::unordered_map<std::type_index,
                       std::unordered_map<uint64, std::unique_ptr<Component>>>
        components;
};
#endif  // __SCENE_H__