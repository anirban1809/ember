#include "ECS/Entity.h"
#include "ECS/Entity.inl"
#include "ECS/Scene.h"

#include "../../../include/ECS/Systems/SkyboxSystem.h"
#include "../../../include/ECS/Components/SkyboxComponent.h"
#include "Core/ShaderProgram.h"
#include "Core/ShaderRegistry.h"
#include "ECS/Components/CameraComponent.h"
#include "GL/glew.h"

void SkyboxSystem::Init(Scene& scene) {
    for (Entity e : scene.View<SkyboxComponent>()) {
        auto& component = e.GetComponent<SkyboxComponent>();
        component.texture = TextureCube::CreateFromCrossLayout(
            "/Users/anirban/Documents/Code/engine/editor/Cubemaps/"
            "plainscubemap.png");
    }

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

    VertexLayout layout = {{ShaderType::Float3, 0, 0}};

    auto vbo = VertexBuffer::Create(
        vertices.data(), vertices.size() * sizeof(float), layout.GetStride());

    vao = VertexArray::Create();
    vao->AddVertexBuffer(vbo);
    vao->SetVertexLayout(layout);
}

void SkyboxSystem::Render(Scene& scene) {
    glm::mat4 view;
    glm::mat4 projection;

    for (Entity e : scene.View<CameraComponent>()) {
        auto& camera = e.GetComponent<CameraComponent>();
        view = glm::mat4(glm::mat3(camera.view));
        projection = camera.projection;
        break;
    }

    for (Entity e : scene.View<SkyboxComponent>()) {
        auto& component = e.GetComponent<SkyboxComponent>();
        auto shader = ShaderRegistry::GetShader("skybox");

        shader->Bind();
        shader->SetUniformMat4("view", view);
        shader->SetUniformMat4("projection", projection);
        shader->SetUniformInt("u_Skybox", 0);

        component.texture->Bind(0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_RenderContext->SubmitArrays(vao, shader, 36);

        break;
    }
}