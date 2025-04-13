#ifndef __LIGHTCOMPONENT_H__
#define __LIGHTCOMPONENT_H__

#include "ECS/Component.h"
#include "glm/glm.hpp"

class LightComponent : public Component {
   public:
    glm::vec3 lightPosition;
    glm::vec3 lightColor;
};
#endif  // __LIGHTCOMPONENT_H__