
#include "../../../../include/Core/Platform/OpenGL/OpenGLVertexBuffer.h"
#include "../../../../../vendor/glew-2.2.0/include/GL/glew.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint64 size,
                                       uint32 stride)
    : stride(stride) {
    glGenBuffers(1, &rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &rendererID); }

void OpenGLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::SetData(const void* data, uint64 size) {
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

uint32 OpenGLVertexBuffer::GetStride() const { return stride; }