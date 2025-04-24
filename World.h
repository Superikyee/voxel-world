#ifndef WORLD_H
#define WORLD_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <array>

#include "VariableTypes.h"

class Vertice;
class Chunk;
class Player;

class World
{
  public:
    World();

    void addRemoveChunks(Player& player, int renderRadius);

    Chunk* getChunkFromPos(glm::ivec2 position);
    Chunk* getChunkFromWorldPos(glm::ivec3 pos);
    std::array<Chunk*, Side::NumSides> getNearbyChunks(glm::ivec2 position);

    void setBlock(BlockInt Block, int PosX, int PosY, int PosZ);
    BlockInt getBlock(int PosX, int PosY, int PosZ);

    std::vector<Chunk> chunks;
  protected:

  private:
};

#endif // WORLD_H
