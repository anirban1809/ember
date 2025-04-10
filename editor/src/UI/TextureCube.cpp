#include "UI/TextureCube.h"
#include "Core/RenderAPI.h"
#include "Core/Types.h"
#include "UI/Platform/OpenGL/OpenGLTextureCube.h"
#include <memory>

std::shared_ptr<TextureCube> TextureCube::CreateFromCrossLayout(
    const std::string &path) {
    if (RenderAPI::Get() == GraphicsAPI::OpenGL) {
        return OpenGLTextureCube::CreateFromCrossLayout(path);
    }
    return nullptr;
}