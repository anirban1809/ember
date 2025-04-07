#ifndef __OPENGLSHADERPROGRAM_H__
#define __OPENGLSHADERPROGRAM_H__

#include "../../ShaderProgram.h"
#include <unordered_map>

class OpenGLShaderProgram : public ShaderProgram {
   public:
    OpenGLShaderProgram(const std::string& vertexSrc,
                        const std::string& fragmentSrc);
    ~OpenGLShaderProgram();

    void Bind() const override;
    void Unbind() const override;

    void SetUniformMat4(const std::string& name,
                        const glm::mat4& value) override;
    void SetUniformFloat(const std::string& name, float value) override;
    void SetUniformFloat3(const std::string& name,
                          const glm::vec3& value) override;
    void SetUniformInt(const std::string& name, int32 value) override;

   private:
    uint32 rendererID;
    mutable std::unordered_map<std::string, int32> uniformLocationCache;

    int32 GetUniformLocation(const std::string& name) const;

    uint32 CompileShader(uint32 type, const std::string& source);
    uint32 LinkProgram(uint32 vertexShader, uint32 fragmentShader);
};
#endif  // __OPENGLSHADERPROGRAM_H__