#ifndef __SHADERREGISTRY_H__
#define __SHADERREGISTRY_H__

#include "Core/ShaderProgram.h"
#include <memory>
#include <string>
#include <unordered_map>

class ShaderRegistry {
   public:
    static void AddShader(const std::string&, const std::string&,
                          const std::string&);
    static std::shared_ptr<ShaderProgram> GetShader(const std::string&);

   private:
    static std::unordered_map<std::string, std::shared_ptr<ShaderProgram>>
        shaders;
};

#endif  // __SHADERREGISTRY_H__