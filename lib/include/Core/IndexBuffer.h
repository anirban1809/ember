#ifndef __INDEXBUFFER_H__
#define __INDEXBUFFER_H__

#include "Types.h"

class IndexBuffer {
   public:
    virtual ~IndexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32 GetCount() const = 0;
};
#endif  // __INDEXBUFFER_H__