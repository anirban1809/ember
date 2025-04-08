#include "../../../../include/Core/Platform/OpenGL/OpenGLShaderProgram.h"
#include "../../../../../vendor/glew-2.2.0/include/GL/glew.h"
#include <iostream>
#include <fstream>
#include <sstream>

OpenGLShaderProgram::OpenGLShaderProgram(const std::string& vertexSrc,
                                         const std::string& fragmentSrc) {
    std::string vertexCode = LoadShaderSource(vertexSrc);
    std::string fragmentCode = LoadShaderSource(fragmentSrc);

    uint32 vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
    uint32 fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);
    rendererID = LinkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

OpenGLShaderProgram::~OpenGLShaderProgram() { glDeleteProgram(rendererID); }

void OpenGLShaderProgram::Bind() const { glUseProgram(rendererID); }

void OpenGLShaderProgram::Unbind() const { glUseProgram(0); }

void OpenGLShaderProgram::SetUniformMat4(const std::string& name,
                                         const glm::mat4& value) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void OpenGLShaderProgram::SetUniformFloat(const std::string& name,
                                          float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShaderProgram::SetUniformFloat3(const std::string& name,
                                           const glm::vec3& value) {
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShaderProgram::SetUniformInt(const std::string& name, int32 value) {
    glUniform1i(GetUniformLocation(name), value);
}

int32 OpenGLShaderProgram::GetUniformLocation(const std::string& name) const {
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];

    int32 location = glGetUniformLocation(rendererID, name.c_str());
    if (location == -1) {
        std::cerr << "[OpenGLShaderProgram] Warning: uniform '" << name
                  << "' not found.\n";
    }

    uniformLocationCache[name] = location;
    return location;
}

uint32 OpenGLShaderProgram::CompileShader(uint32 type,
                                          const std::string& source) {
    uint32 shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "[OpenGLShaderProgram] Shader compilation error:\n"
                  << infoLog << "\n";
    }

    return shader;
}

uint32 OpenGLShaderProgram::LinkProgram(uint32 vertexShader,
                                        uint32 fragmentShader) {
    uint32 program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "[OpenGLShaderProgram] Program link error:\n"
                  << infoLog << "\n";
    }

    return program;
}