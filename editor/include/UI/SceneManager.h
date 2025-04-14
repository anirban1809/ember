#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Core/Types.h"
#include "ECS/Scene.h"
#include <memory>
#include <string>

class SceneManager {
   public:
    SceneManager(Scene& scene) : m_ActiveScene(scene) {}
    Scene& GetActiveScene() { return m_ActiveScene; }

    // scene operations
    void RenameScene(const std::string&);

    // scene file operations
    void SaveSceneToFile(const std::string&);
    void OpenSceneFile(const std::string&);

    // entity operations
    Entity CreateEntity(const std::string&);
    void DestroyEntity(Entity entity);
    void RenameEntity(Entity entity, const std::string&);

    // selection api
    void SelectEntity(uint64);
    Entity GetSelectedEntity();

    // querying
    std::unordered_set<uint64> GetAllEntities() const;
    std::string GetEntityName(uint64 id) const;
    std::vector<Component> GetComponents(Entity);

   private:
    Scene& m_ActiveScene;
    Entity m_SelectedEntity;
    std::string m_SceneName;
};
#endif  // __SCENEMANAGER_H__