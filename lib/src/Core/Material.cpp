#include "../../include/Core/Material.h"

Material::Material(std::shared_ptr<ShaderProgram> shader) : shader(shader) {}

void Material::SetFloat(const std::string& name, float value) {
    floatUniforms.emplace_back(name, value);
}

void Material::SetFloat3(const std::string& name, const glm::vec3& value) {
    float3Uniforms.emplace_back(name, value);
}

void Material::SetMat4(const std::string& name, const glm::mat4& value) {
    mat4Uniforms.emplace_back(name, value);
}

void Material::SetInt(const std::string& name, int32 value) {
    intUniforms.emplace_back(name, value);
}

void Material::Apply() { shader->Bind(); }

std::shared_ptr<ShaderProgram> Material::GetShader() const { return shader; }