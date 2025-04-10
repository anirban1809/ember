#ifndef __OPENGLTEXTURECUBE_H__
#define __OPENGLTEXTURECUBE_H__

#include "../../TextureCube.h"
#include "Core/Types.h"
#include <memory>

class OpenGLTextureCube : public TextureCube {
   public:
    OpenGLTextureCube(uint32 resolution);
    OpenGLTextureCube(uint32 id, int faceSize) : id(id), size(faceSize) {}

    void Bind(uint32 slot = 0) const override;
    void BindFaceForWriting(uint32 face) const override;
    static std::shared_ptr<TextureCube> CreateFromCrossLayout(
        const std::string&);

   private:
    uint32 id;
    int32 size;
};
#endif  // __OPENGLTEXTURECUBE_H__