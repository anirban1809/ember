#ifndef __OPENGLINDEXBUFFER_H__
#define __OPENGLINDEXBUFFER_H__

#include "../../IndexBuffer.h"

class OpenGLIndexBuffer : public IndexBuffer {
   public:
    OpenGLIndexBuffer(const uint32* indices, uint32 count);
    ~OpenGLIndexBuffer();

    void Bind() const override;
    void Unbind() const override;

    uint32 GetCount() const override;

   private:
    uint32 rendererID;
    uint32 count;
};
#endif  // __OPENGLINDEXBUFFER_H__