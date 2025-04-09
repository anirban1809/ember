#include "../../../../include/Core/Platform/OpenGL/OpenGLRenderContext.h"
#include "../../../../include/Core/Platform/OpenGL/OpenGLVertexBuffer.h"
#include "../../../../include/Core/Platform/OpenGL/OpenGLIndexBuffer.h"
#include "../../../../include/Core/Platform/OpenGL/OpenGLVertexArray.h"
#include "../../../../include/Core/Platform/OpenGL/OpenGLShaderProgram.h"
#include "../../../../../vendor/glew-2.2.0/include/GL/glew.h"
#include "Core/ShaderProgram.h"
#include <iostream>
#include <memory>

void OpenGLRenderContext::Submit(const Mesh& mesh) {
    mesh.material->Apply();

    auto shader = mesh.material->GetShader();
    shader->SetUniformMat4("model", mesh.transform);

    mesh.vao->Bind();
    glDrawElements(GL_TRIANGLES, mesh.ibo->GetCount(), GL_UNSIGNED_INT,
                   nullptr);
    mesh.vao->Unbind();
}

void OpenGLRenderContext::SubmitArrays(std::shared_ptr<VertexArray> vao,
                                       std::shared_ptr<ShaderProgram> shader,
                                       uint32 vertexCount) {
    shader->Bind();
    vao->Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    vao->Unbind();
}

void OpenGLRenderContext::BeginScene(
    std::vector<std::shared_ptr<ShaderProgram>>& shaders) {
    SetGlobalShaders(shaders);
    for (auto const& shader : shaders) {
        shader->Bind();
    }

    glEnable(GL_CULL_FACE);
    // Cull back faces
    glCullFace(GL_BACK);
    // Specify front face winding order (optional, depending on your vertex
    // order)
    glFrontFace(GL_CCW);
}

void OpenGLRenderContext::SetGlobalShaders(
    const std::vector<std::shared_ptr<ShaderProgram>>& shaders) {
    globalShaders = shaders;
}

/**@deprecated */
void OpenGLRenderContext::Submit(const std::vector<float>& vertices,
                                 const std::vector<uint32>& indices,
                                 const VertexLayout& layout,
                                 const glm::mat4& transform) {}

void OpenGLRenderContext::EndScene() {
    // In current setup, everything is rendered immediately in Submit().
    // If batching is added, loop and draw here.
}

void OpenGLRenderContext::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}