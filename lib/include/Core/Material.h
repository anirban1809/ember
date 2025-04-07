#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "ShaderProgram.h"
#include <vector>

class Material {
   public:
    Material(std::shared_ptr<ShaderProgram> shader);

    void SetFloat(const std::string& name, float value);
    void SetFloat3(const std::string& name, const glm::vec3& value);
    void SetMat4(const std::string& name, const glm::mat4& value);
    void SetInt(const std::string& name, int32 value);

    void Apply();

    std::shared_ptr<ShaderProgram> GetShader() const;

   private:
    std::shared_ptr<ShaderProgram> shader;

    // Simple uniform cache
    std::vector<std::pair<std::string, float>> floatUniforms;
    std::vector<std::pair<std::string, glm::vec3>> float3Uniforms;
    std::vector<std::pair<std::string, glm::mat4>> mat4Uniforms;
    std::vector<std::pair<std::string, int32>> intUniforms;
};
#endif  // __MATERIAL_H__