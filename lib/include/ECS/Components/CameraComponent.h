#ifndef __CAMERACOMPONENT_H__
#define __CAMERACOMPONENT_H__

#include "ECS/Component.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

struct CameraComponent : public Component {
    glm::vec3 cameraPosition;
    glm::vec3 cameraLook;
    glm::mat4 projection;
    glm::mat4 view;

    CameraComponent(glm::vec3 position, glm::vec3 look, glm::vec4 projection)
        : cameraPosition(position),
          cameraLook(look),
          projection(glm::perspective(glm::radians(projection[0]),
                                      projection[1], projection[2],
                                      projection[3])) {
        view = glm::lookAt(cameraPosition, cameraLook,
                           glm::vec3(0.0f, 1.0f, 0.0f));
    }
};
#endif  // __CAMERACOMPONENT_H__