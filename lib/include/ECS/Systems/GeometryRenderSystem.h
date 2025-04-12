#ifndef __RENDERSYSTEM_H__
#define __RENDERSYSTEM_H__

#include "Core/ShaderProgram.h"
#include "ECS/System.h"
#include <memory>
class GeometryRenderSystem : public System {
   public:
    void Init(Scene&) override;
    void Update(Scene&) override;
    std::string GetName() const override;
};
#endif  // __RENDERSYSTEM_H__