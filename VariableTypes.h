#ifndef VARIABLETYPES_H_INCLUDED
#define VARIABLETYPES_H_INCLUDED

#include <cstdint>
#include <string>

  typedef uint8_t BlockInt;

  struct BlockType
  {
    enum Type
    {
      Cube,
      Water,
      NumTypes
    };
  };

  struct Blocks
  {
    enum Name
    {
      Air,
      Grass,
      Water,
      Sand,
      Dirt,
      Clay,
      Log,
      Leaves
    };
  };

  struct ShaderType
  {
    enum Type
    {
      CubeShader,
      WaterShader,
      NumTypes
    };
};

namespace Data
{
  //TODO: Make it read from file

  //blocks - update these constants when adding new blocks, and also update block name enum!
  const unsigned int numBlocks = 8;

  const BlockType::Type blockType[numBlocks] =
  {
    BlockType::NumTypes, BlockType::Cube, BlockType::Water, BlockType::Cube, BlockType::Cube, BlockType::Cube, BlockType::Cube, BlockType::Cube
  };
  const bool isTransparent[numBlocks] =
  {
    true, false, true, false, false, false, false, false
  };
  const bool canUseCullFace[numBlocks] =
  {
    false, true, false, true, true, true, true, true
  };


  //shaders
  const ShaderType::Type shaderType[BlockType::NumTypes] =
  {
    ShaderType::CubeShader, ShaderType::WaterShader
  };
  const std::string shaderPath[ShaderType::NumTypes*2] =
  {
    "shader/CubeShader.vert", "shader/CubeShader.frag", "shader/WaterShader.vert", "shader/WaterShader.frag"
  };
}

struct Dimension
{
  enum Type
  {
    X,
    Y,
    Z
  };
};

struct BlockFace {
  enum Type
  {
    X_PLUS,
    X_MIN,
    TOP,
    BOTTOM,
    Z_PLUS,
    Z_MIN,
    NumFaces
  };
};

struct Side
{
  enum Type
  {
    X_PLUS,
    X_MIN,
    Y_PLUS,
    Y_MIN,
    NumSides
  };
};

#endif // VARIABLETYPES_H_INCLUDED
