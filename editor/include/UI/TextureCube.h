#ifndef __TEXTURECUBE_H__
#define __TEXTURECUBE_H__

#include "Core/Types.h"
#include <memory>
#include <string>

class TextureCube {
   public:
    virtual ~TextureCube() = default;

    virtual void Bind(uint32 slot = 0) const = 0;
    virtual void BindFaceForWriting(uint32 face) const = 0;
    static std::shared_ptr<TextureCube> CreateFromCrossLayout(
        const std::string&);
};
#endif  // __TEXTURECUBE_H__