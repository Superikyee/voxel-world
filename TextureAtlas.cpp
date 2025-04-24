#include "TextureAtlas.h"

#include <iostream>

TextureAtlas::TextureAtlas(const char* TexFilename)
{
  atlasTexture.load(TexFilename);
}


glm::vec4 TextureAtlas::getTexArea(BlockInt Block, BlockFace::Type Face)
{
  if (Face == BlockFace::TOP) {
    return glm::vec4(Block * texSize, texSize*2, texSize, texSize);
  } else if (Face == BlockFace::X_PLUS || Face == BlockFace::X_MIN || Face == BlockFace::Z_PLUS || Face == BlockFace::Z_MIN) {
    return glm::vec4(Block * texSize, texSize, texSize, texSize);
  } else if (Face == BlockFace::BOTTOM) {
    return glm::vec4(Block * texSize, 0, texSize, texSize);
  }
  return glm::vec4(0, 0, 0, 0);
}

std::vector<float> TextureAtlas::getFaceTexCoords(BlockInt Block, BlockFace::Type Face)
{
  std::vector<float> output(12);

  glm::vec4 Area = getTexArea(Block, Face);
  if (Face == BlockFace::TOP) {
    Area.y++;
    Area.w-= 2;
    output[0] = Area.x; output[1] = Area.y + Area.w;
    output[2] = Area.x + Area.z; output[3] = Area.y + Area.w;
    output[4] = Area.x + Area.z; output[5] = Area.y;
    output[6] = Area.x + Area.z; output[7] = Area.y;
    output[8] = Area.x; output[9] = Area.y;
    output[10] = Area.x; output[11] = Area.y + Area.w;
  } else if (Face == BlockFace::X_PLUS || Face == BlockFace::X_MIN) {
    output[0] = Area.x + Area.z; output[1] = Area.y + Area.w;
    output[2] = Area.x; output[3] = Area.y + Area.w;
    output[4] = Area.x; output[5] = Area.y;
    output[6] = Area.x; output[7] = Area.y;
    output[8] = Area.x + Area.z; output[9] = Area.y;
    output[10] = Area.x + Area.z; output[11] = Area.y + Area.w;
  } else if (Face == BlockFace::Z_MIN || Face == BlockFace::Z_PLUS) {
    output[0] = Area.x; output[1] = Area.y;
    output[2] = Area.x + Area.z; output[3] = Area.y;
    output[4] = Area.x + Area.z; output[5] = Area.y + Area.w;
    output[6] = Area.x + Area.z; output[7] = Area.y + Area.w;
    output[8] = Area.x; output[9] = Area.y + Area.w;
    output[10] = Area.x; output[11] = Area.y;
  } else if (Face == BlockFace::BOTTOM) {
    Area.y++;
    Area.w-= 2;
    output[0] = Area.x + Area.z; output[1] = Area.y;
    output[2] = Area.x; output[3] = Area.y;
    output[4] = Area.x; output[5] = Area.y + Area.w;;
    output[6] = Area.x; output[7] = Area.y + Area.w;
    output[8] = Area.x + Area.z; output[9] = Area.y + Area.w;;
    output[10] = Area.x + Area.z; output[11] = Area.y;
  }

  for (unsigned int i = 0; i < 12; i++) {
    output[i] /= (i % 2 == 0) ? (atlasTexture.width) : (atlasTexture.height);
  }

  return output;
}


