#ifndef __SKYBOXSYSTEM_H__
#define __SKYBOXSYSTEM_H__

#include "Core/RenderContext.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "ECS/System.h"
#include <memory>
#include <string>
class SkyboxSystem : public System {
   public:
    SkyboxSystem(std::shared_ptr<RenderContext> context)
        : m_RenderContext(context) {}
    void Init(Scene&);
    void Render(Scene&);

   private:
    std::shared_ptr<RenderContext> m_RenderContext;
    std::shared_ptr<VertexArray> vao;
};

#endif  // __SKYBOXSYSTEM_H__