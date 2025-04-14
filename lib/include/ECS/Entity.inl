#include "Scene.h"
#include <utility>

template <typename T, typename... Args>
T& Entity::AddComponent(Args&&... args) {
    return scene->AddComponent<T>(*this, std::forward<Args>(args)...);
}

template <typename T>
T& Entity::AddComponent(T& component) {
    return scene->AddComponent<T>(*this, component);
}

template <typename T>
T& Entity::GetComponent() {
    return scene->GetComponent<T>(*this);
}

template <typename T>
bool Entity::HasComponent() const {
    return scene->HasComponent<T>(*this);
}

template <typename T>
void Entity::RemoveComponent() {
    scene->RemoveComponent<T>(*this);
}