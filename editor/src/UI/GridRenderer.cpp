
#include "UI/GridRenderer.h"
#include "Core/Camera.h"
#include "Core/RenderContext.h"
#include "Core/VertexLayout.h"
GridRenderer::GridRenderer() { InitFullscreenTriangle(); }

void GridRenderer::InitFullscreenTriangle() {
    std::vector<float> vertices = {-1.0f, -1.0f, 3.0f, -1.0f, -1.0f, 3.0f};

    std::vector<uint32> indices = {0, 1, 2};

    VertexLayout layout = {
        {ShaderType::Float2, 0, 0}  // aPosition at location = 0
    };

    auto vbo = VertexBuffer::Create(
        vertices.data(), vertices.size() * sizeof(float), layout.GetStride());
    auto ibo = IndexBuffer::Create(indices.data(), indices.size());

    vao = VertexArray::Create();
    vao->AddVertexBuffer(vbo);
    vao->SetVertexLayout(layout);
    vao->SetIndexBuffer(ibo);
}

void GridRenderer::Render(std::shared_ptr<RenderContext> context,
                          std::shared_ptr<ShaderProgram> shader, Camera& camera,
                          float majorSpacing, float minorSpacing,
                          const glm::vec3& gridColor) {
    glm::mat4 invView = glm::inverse(camera.GetView());
    glm::mat4 invProjection = glm::inverse(camera.GetProjection());

    shader->Bind();
    shader->SetUniformMat4("invView", invView);
    shader->SetUniformMat4("invProjection", invProjection);
    shader->SetUniformFloat3("cameraPos", camera.GetCameraPosition());
    shader->SetUniformFloat("majorLineSpacing", majorSpacing);
    shader->SetUniformFloat("minorLineSpacing", minorSpacing);
    shader->SetUniformFloat3("gridColor", gridColor);

    context->SubmitArrays(
        vao, shader, 3);  // non-indexed is also fine here, but we're using IBO
}
