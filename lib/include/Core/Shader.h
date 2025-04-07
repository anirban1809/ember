#ifndef SHADER_H
#define SHADER_H

#include "Types.h"
#include <string>
#include "../../../vendor/glm/glm.hpp"

enum class ShaderAPI {
    None = 0,
    OpenGL = 1,
    DirectX = 2,
    Vulkan = 3,
    GMMX = 4,
};

class Shader {
   public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    int32 GetUniformLocation(uint32 programId, const char *uniformName);

    void SetShaderUniformInt(int32 location, int32 value);
    void SetShaderUniform3Float(int32 location, float, float, float);
    void SetShaderUniformMat4Float(int32, glm::mat4 &);
    void SetShaderUniformVec3Float(int32, glm::vec3 &);
    void SetShaderUniformFloat(int32, float);

    std::string LoadShaderSource(const std::string &filePath);
    uint32 CompileShader(uint32 type, const std::string &source);
    void Use() const;
    uint32 GetProgramId();

    ~Shader();

   private:
    uint32 programID;
};

#endif
