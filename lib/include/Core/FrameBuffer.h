#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "Types.h"
#pragma once

#include "../Core/Types.h"
#include <memory>

class FrameBuffer {
   public:
    virtual ~FrameBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void Clear() const = 0;

    virtual void Resize(float width, float height) = 0;

    virtual uint32 GetColorAttachmentTexture() const = 0;

    static std::shared_ptr<FrameBuffer> Create(float width, float height);
};

#endif  // __FRAMEBUFFER_H__