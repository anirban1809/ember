#pragma once

#include "Core/Camera.h"
#include "Core/RenderContext.h"
#include <glm/glm.hpp>
#include <memory>

class GridRenderer {
   public:
    GridRenderer();

    void Render(std::shared_ptr<RenderContext> context,
                std::shared_ptr<ShaderProgram> shader, Camera& camera,
                float majorSpacing = 1.0f, float minorSpacing = 0.1f,
                const glm::vec3& gridColor = glm::vec3(0.3f));

   private:
    std::shared_ptr<Material> material;
    std::shared_ptr<VertexArray> vao;

    void InitFullscreenTriangle();
};
