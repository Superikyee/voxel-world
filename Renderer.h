#ifndef RENDERER_H
#define RENDERER_H

#include <array>

#include "VariableTypes.h"

class World;
class Shader;
class WorldShaders;
class Player;

class Renderer
{
  public:
    Renderer();
    void drawChunks(World& m_database, WorldShaders& m_shaders);
    void setMatrices(WorldShaders& m_shaders, Player& player, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);

    std::array<unsigned int, BlockType::NumTypes> VBO;
    std::array<unsigned int, BlockType::NumTypes> VAO;
  protected:

  private:
};

#endif // RENDERER_H
