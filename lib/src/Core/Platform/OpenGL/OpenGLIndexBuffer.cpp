#include "../../../../include/Core/Platform/OpenGL/OpenGLIndexBuffer.h"
#include "../../../../../vendor/glew-2.2.0/include/GL/glew.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32* indices, uint32 count)
    : count(count) {
    glGenBuffers(1, &rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32), indices,
                 GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &rendererID); }

void OpenGLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void OpenGLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32 OpenGLIndexBuffer::GetCount() const { return count; }