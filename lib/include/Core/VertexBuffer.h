#ifndef __VERTEXBUFFER_H__
#define __VERTEXBUFFER_H__

#include "Types.h"

class VertexBuffer {
   public:
    virtual ~VertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(const void* data, uint64 size) = 0;
    virtual uint32 GetStride() const = 0;
};

#endif  // __VERTEXBUFFER_H__