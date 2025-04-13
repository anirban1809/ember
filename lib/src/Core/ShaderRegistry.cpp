#include "Core/ShaderRegistry.h"
#include "Core/ShaderProgram.h"
#include <memory>
#include <string>

std::unordered_map<std::string, std::shared_ptr<ShaderProgram>>
    ShaderRegistry::shaders;

void ShaderRegistry::AddShader(const std::string& name,
                               const std::string& vertexPath,
                               const std::string& fragmentPath) {
    shaders[name] = ShaderProgram::Create(vertexPath, fragmentPath);
}

std::shared_ptr<ShaderProgram> ShaderRegistry::GetShader(
    const std::string& name) {
    return shaders[name];
}
