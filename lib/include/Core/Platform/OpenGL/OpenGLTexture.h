#ifndef __OPENGLTEXTURE_H__
#define __OPENGLTEXTURE_H__
#define STB_IMAGE_IMPLEMENTATION
#include "Core/Texture.h"

class OpenGLTexture : public Texture {
   public:
    OpenGLTexture(const std::string& path,
                  TextureType type = TextureType::Unknown, bool srgb = false);
    virtual ~OpenGLTexture() override;

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Unbind() const override;

    virtual uint32_t GetID() const override;
    virtual TextureType GetType() const override;
    virtual const std::string& GetPath() const override;

   private:
    uint32 rendererId = 0;
    std::string path;
    TextureType type;

    void LoadFromFile(const std::string& path, bool srgb);
};
#endif  // __OPENGLTEXTURE_H__