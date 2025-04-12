#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Core/Types.h"

class Scene;
class Entity {
   public:
    Entity(uint32, Scene*);
    inline uint32 GetId() { return entityId; }

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args);

    template <typename T>
    T& GetComponent();

    template <typename T>
    bool HasComponent() const;

    template <typename T>
    void RemoveComponent();

   private:
    uint64 entityId;
    Scene* scene;
};
#endif  // __ENTITY_H__