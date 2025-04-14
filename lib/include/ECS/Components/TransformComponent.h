#ifndef __TRANSFORMCOMPONENT_H__
#define __TRANSFORMCOMPONENT_H__

#include "ECS/Component.h"
#include <glm/glm.hpp>

struct TransformComponent : public Component {
   public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};
#endif  // __TRANSFORMCOMPONENT_H__