#ifndef __TRANSFORMCOMPONENT_H__
#define __TRANSFORMCOMPONENT_H__

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ECS/Component.h"

struct TransformComponent : public Component {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);  // In degrees
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 GetTransform() const {
        glm::mat4 mat = glm::mat4(1.0f);

        mat = glm::translate(mat, position);

        mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));

        mat = glm::scale(mat, scale);
        return mat;
    }
};

#endif  // __TRANSFORMCOMPONENT_H__