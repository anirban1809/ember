#ifndef __GRIDSYSTEM_H__
#define __GRIDSYSTEM_H__

#include "Core/RenderContext.h"
#include "Core/VertexArray.h"
#include "ECS/System.h"

class GridSystem : public System {
   public:
    GridSystem(std::shared_ptr<RenderContext> context);
    void Render(Scene&);

   private:
    std::shared_ptr<RenderContext> m_RenderContext;
    std::shared_ptr<VertexArray> vao;
    void InitFullscreenTriangle();
};
#endif  // __GRIDSYSTEM_H__