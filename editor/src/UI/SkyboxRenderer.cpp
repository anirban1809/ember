#include "UI/SkyboxRenderer.h"
#include "Core/FrameBuffer.h"
#include "UI/TextureCube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

SkyboxRenderer::SkyboxRenderer() { InitCube(); }

void SkyboxRenderer::InitCube() {
    std::vector<float> vertices = {
        // Back face
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,

        // Left face
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,

        // Right face
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,

        // Front face
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,

        // Top face
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,

        // Bottom face
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};

    indexCount = 36;

    VertexLayout layout = {{ShaderType::Float3, 0, 0}};

    auto vbo = VertexBuffer::Create(
        vertices.data(), vertices.size() * sizeof(float), layout.GetStride());
    // auto ibo = IndexBuffer::Create(indices.data(), indices.size());

    vao = VertexArray::Create();
    vao->AddVertexBuffer(vbo);
    vao->SetVertexLayout(layout);
    // vao->SetIndexBuffer(ibo);
}

void SkyboxRenderer::Render(std::shared_ptr<RenderContext> context,
                            std::shared_ptr<ShaderProgram> shader,
                            std::shared_ptr<TextureCube> cubemap,
                            Camera& camera) {
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetView()));
    glm::mat4 projection = camera.GetProjection();

    shader->Bind();
    shader->SetUniformMat4("view", view);
    shader->SetUniformMat4("projection", projection);
    shader->SetUniformInt("u_Skybox", 0);

    cubemap->Bind(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    context->SubmitArrays(vao, shader, indexCount);
}