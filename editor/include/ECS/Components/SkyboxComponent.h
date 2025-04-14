#ifndef __SKYBOXCOMPONENT_H__
#define __SKYBOXCOMPONENT_H__

#include "ECS/Component.h"
#include <memory>
#include "../../UI/TextureCube.h"

struct SkyboxComponent : public Component {
    std::shared_ptr<TextureCube> texture;
    SkyboxComponent(const std::string& path) {
        texture = TextureCube::CreateFromCrossLayout(path);
    }
};
#endif  // __SKYBOXCOMPONENT_H__