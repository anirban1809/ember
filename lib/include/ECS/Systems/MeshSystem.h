#ifndef __RENDERSYSTEM_H__
#define __RENDERSYSTEM_H__

#include "Core/RenderContext.h"
#include "Core/ShaderProgram.h"
#include "ECS/Scene.h"
#include "ECS/System.h"
#include <memory>
class MeshSystem : public System {
   public:
    MeshSystem(std::shared_ptr<RenderContext>& context)
        : m_RenderContext(context) {}
    void Init(Scene&);
    void Render(Scene&);

   private:
    std::shared_ptr<RenderContext> m_RenderContext;
};
#endif  // __RENDERSYSTEM_H__