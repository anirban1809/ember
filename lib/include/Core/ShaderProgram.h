#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#include <string>
#include "../../../vendor/glm/glm.hpp"
#include "Types.h"

class ShaderProgram {
   public:
    virtual ~ShaderProgram() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetUniformMat4(const std::string& name,
                                const glm::mat4& value) = 0;
    virtual void SetUniformFloat(const std::string& name, float value) = 0;
    virtual void SetUniformFloat3(const std::string& name,
                                  const glm::vec3& value) = 0;
    virtual void SetUniformInt(const std::string& name, int32 value) = 0;
    std::string LoadShaderSource(const std::string&);

    static std::shared_ptr<ShaderProgram> Create(const std::string&,
                                                 const std::string&);
};
#endif  // __SHADERPROGRAM_H__