#include "Core/Texture.h"
#include "Core/Platform/OpenGL/OpenGLTexture.h"
#include "Core/RenderAPI.h"
#include <memory>

std::shared_ptr<Texture> Texture::Create(const std::string &path,
                                         TextureType type, bool srgb) {
    switch (RenderAPI::Get()) {
        case GraphicsAPI::None:
            return nullptr;
        case GraphicsAPI::OpenGL:
            return std::make_shared<OpenGLTexture>(path, type, srgb);
            // case GraphicsAPI::Vulkan:
        default:
            return nullptr;
    }
}