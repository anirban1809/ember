#include "../../../../include/Core/Platform/OpenGL/OpenGLVertexArray.h"
#include "../../../../../vendor/glew-2.2.0/include/GL/glew.h"

OpenGLVertexArray::OpenGLVertexArray() {
    glGenVertexArrays(1, &rendererID);
    glBindVertexArray(rendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &rendererID);
}

void OpenGLVertexArray::Bind() const { glBindVertexArray(rendererID); }

void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }

void OpenGLVertexArray::AddVertexBuffer(
    std::shared_ptr<VertexBuffer> vertexBuffer) {
    Bind();
    vertexBuffer->Bind();
    vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetVertexLayout(const VertexLayout& layout) {
    Bind();
    uint32 stride = layout.GetStride();

    for (const auto& attribute : layout.GetAttributes()) {
        glEnableVertexAttribArray(attribute.location);
        GLint componentCount = 0;
        GLenum type = GL_FLOAT;

        switch (attribute.type) {
            case ShaderType::Float:
                componentCount = 1;
                break;
            case ShaderType::Float2:
                componentCount = 2;
                break;
            case ShaderType::Float3:
                componentCount = 3;
                break;
            case ShaderType::Float4:
                componentCount = 4;
                break;
            default:
                break;  // Extend for more types if needed
        }

        glVertexAttribPointer(attribute.location, componentCount, type,
                              GL_FALSE, stride,
                              (const void*)(uintptr_t)attribute.offset);
        attributeIndex++;
    }
}

void OpenGLVertexArray::SetIndexBuffer(
    std::shared_ptr<IndexBuffer> indexBuffer) {
    Bind();
    indexBuffer->Bind();
    this->indexBuffer = indexBuffer;
}

const std::vector<std::shared_ptr<VertexBuffer>>&
OpenGLVertexArray::GetVertexBuffers() const {
    return vertexBuffers;
}

const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const {
    return indexBuffer;
}