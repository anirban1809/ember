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

    mesh.vao->Bind();
    glDrawElements(GL_TRIANGLES, mesh.ibo->GetCount(), GL_UNSIGNED_INT,
                   nullptr);
    mesh.vao->Unbind();
}

void OpenGLRenderContext::BeginScene(
    std::vector<std::shared_ptr<ShaderProgram>>& shaders) {
    for (auto const& shader : shaders) {
        shader->Bind();
    }
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