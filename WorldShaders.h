#ifndef WORLDSHADERS_H
#define WORLDSHADERS_H

#include <string>
#include <array>

#include "Shader.h"
#include "VariableTypes.h"

class WorldShaders
{
  public:
    
    WorldShaders() {
      for (unsigned int i = 0; i < ShaderType::NumTypes; i++) {
        Shaders[ShaderType::Type(i)].generate(Data::shaderPath[i*2].c_str(), Data::shaderPath[i*2+1].c_str());
      }
    }

    Shader& operator[](ShaderType::Type type)
    {
      return Shaders[type];
    }

    std::array<Shader, ShaderType::NumTypes> Shaders;
  protected:

  private:
};

#endif // WORLDSHADERS_H
