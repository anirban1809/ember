#ifndef __MATERIALCOMPONENT_H__
#define __MATERIALCOMPONENT_H__

#include "Core/Texture.h"
#include "ECS/Component.h"
#include <glm/glm.hpp>
#include <unordered_map>

struct MaterialComponent : public Component {
    glm::vec3 albedoColor = glm::vec3(1.0f);
    float metallic = 0.0f;
    float roughness = 1.0f;

    std::unordered_map<std::string, Texture> textures;

    MaterialComponent() = default;
};

#endif  // __MATERIALCOMPONENT_H__