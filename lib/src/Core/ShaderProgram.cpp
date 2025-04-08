#include "../../include/Core/ShaderProgram.h"
#include "../../include/Core/Platform/OpenGL/OpenGLShaderProgram.h"
#include "../../include/Core/RenderAPI.h"
#include <sstream>
#include <fstream>

std::shared_ptr<ShaderProgram> ShaderProgram::Create(
    const std::string& vertexSrc, const std::string& fragmentSrc) {
    switch (RenderAPI::Get()) {
        case GraphicsAPI::OpenGL:
            return std::make_shared<OpenGLShaderProgram>(vertexSrc,
                                                         fragmentSrc);
        // case GraphicsAPI::Vulkan:
        //     return std::make_shared<VulkanRenderContext>();
        default:
            return nullptr;
    }
}

std::string ShaderProgram::LoadShaderSource(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}