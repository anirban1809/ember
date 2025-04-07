#include "../../../../include/Core/Platform/OpenGL/OpenGLRenderContext.h"
#include "../../../../include/Core/Platform/OpenGL/OpenGLVertexBuffer.h"
#include "../../../../include/Core/Platform/OpenGL/OpenGLIndexBuffer.h"
#include "../../../../include/Core/Platform/OpenGL/OpenGLVertexArray.h"
#include "../../../../include/Core/Platform/OpenGL/OpenGLShaderProgram.h"
#include "../../../../../vendor/glew-2.2.0/include/GL/glew.h"

void OpenGLRenderContext::Submit(const Mesh& mesh) {
    mesh.material->Apply();
    auto shader = mesh.material->GetShader();
    shader->SetUniformMat4("u_Model", mesh.transform);
    Submit(mesh.vertices, mesh.indices, mesh.layout, mesh.transform);
}

void OpenGLRenderContext::BeginScene(const glm::mat4& viewProjection,
                                     std::shared_ptr<ShaderProgram> shader) {
    this->viewProjection = viewProjection;
    shader->Bind();
    shader->SetUniformMat4("u_ViewProjection", viewProjection);
}

void OpenGLRenderContext::Submit(const std::vector<float>& vertices,
                                 const std::vector<uint32>& indices,
                                 const VertexLayout& layout,
                                 const glm::mat4& transform) {
    auto vao = std::make_shared<OpenGLVertexArray>();
    auto vbo = std::make_shared<OpenGLVertexBuffer>(
        vertices.data(), vertices.size() * sizeof(float), layout.GetStride());
    auto ibo = std::make_shared<OpenGLIndexBuffer>(indices.data(),
                                                   (uint32)indices.size());

    vao->AddVertexBuffer(vbo);
    vao->SetVertexLayout(layout);
    vao->SetIndexBuffer(ibo);

    // Apply transform matrix
    vao->Bind();
    vao->GetIndexBuffer()->Bind();

    glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, nullptr);

    vao->Unbind();
}

void OpenGLRenderContext::EndScene() {
    // In current setup, everything is rendered immediately in Submit().
    // If batching is added, loop and draw here.
}

void OpenGLRenderContext::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}