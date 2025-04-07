#ifndef __OPENGLVERTEXBUFFER_H__
#define __OPENGLVERTEXBUFFER_H__

#include "../../../Core/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
   public:
    OpenGLVertexBuffer(const void* data, uint64 size, uint32 stride);
    ~OpenGLVertexBuffer();

    void Bind() const override;
    void Unbind() const override;

    void SetData(const void* data, uint64 size) override;
    uint32 GetStride() const override;

   private:
    uint32 rendererID;
    uint32 stride;
};
#endif  // __OPENGLVERTEXBUFFER_H__