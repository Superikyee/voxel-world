#ifndef BLOCKATLAS_H
#define BLOCKATLAS_H

#include <glm/vec4.hpp>

#include <vector>

#include "VariableTypes.h"
#include "Texture.h"
#include "Shader.h"

class TextureAtlas
{
  public:
    TextureAtlas(const char* TexFilename);

    Texture& getTexture() {return atlasTexture;}

    std::vector<float> getFaceTexCoords(BlockInt block, BlockFace::Type face);
    glm::vec4 getTexArea(BlockInt block, BlockFace::Type face);

    const unsigned int texSize = 16;
  protected:

  private:
    Texture atlasTexture;
};

#endif // BLOCKATLAS_H
