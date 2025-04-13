#ifndef __SKYBOXCOMPONENT_H__
#define __SKYBOXCOMPONENT_H__

#include "ECS/Component.h"
#include <memory>
#include "../../UI/TextureCube.h"

struct SkyboxComponent : public Component {
    std::shared_ptr<TextureCube> texture;
    SkyboxComponent(const std::shared_ptr<TextureCube>& texture)
        : texture(texture) {}
};
#endif  // __SKYBOXCOMPONENT_H__