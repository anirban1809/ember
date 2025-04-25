#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Types.h"
#include <memory>
#include <string>

enum class TextureType {
    Albedo,
    Normal,
    Metallic,
    Roughness,
    AO,
    Emissive,
    Unknown
};

class Texture {
   public:
    static std::shared_ptr<Texture> Create(const std::string& path,
                                           TextureType type, bool srgb);
    virtual ~Texture() = default;

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetID() const = 0;
    virtual TextureType GetType() const = 0;
    virtual const std::string& GetPath() const = 0;
};

#endif  // __TEXTURE_H__