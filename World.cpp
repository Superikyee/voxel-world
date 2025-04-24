#include "World.h"

#include <glm/vec3.hpp>

#include <math.h>

#include "Player.h"
#include "Vertice.h"
#include "TextureAtlas.h"
#include "Mesh.h"
#include "TerrainGeneration.h"
#include "Chunk.h"

World::World()
{
  //ctor
}

glm::ivec3 getPosInChunk(int posX, int posY, int posZ) {

  glm::ivec3 pos(posX % chunkWidth, posY, posZ % chunkWidth); //position in chunk

  if (pos.x < 0) {
    pos.x += chunkWidth;
  }
  if (pos.z < 0) {
    pos.z += chunkWidth;
  }

  return pos;
}

///@return Pointer to chunk in that position, nullptr if it doesn't exist
Chunk* World::getChunkFromPos(glm::ivec2 position)
{
  for (unsigned int i = 0; i < chunks.size(); i++) {
    if (chunks[i].getPosition() == position) {
      return &chunks[i];
    }
  }

  return nullptr;
}


Chunk* World::getChunkFromWorldPos(glm::ivec3 pos) {
  int chunkX = std::floor(double(pos.x) / double(chunkWidth));
  int chunkY = std::floor(double(pos.z) / double(chunkWidth));
  
  return getChunkFromPos(glm::ivec2(chunkX, chunkY));
}

void World::setBlock(BlockInt Block, int PosX, int PosY, int PosZ)
{

  Chunk* chunk = getChunkFromWorldPos(glm::ivec3(PosX, PosY, PosZ));

  if (PosY < chunkHeight && chunk != nullptr) {
    glm::ivec3 ChunkPos = getPosInChunk(PosX, PosY, PosZ);
    chunk->setBlock(ChunkPos.x, ChunkPos.y, ChunkPos.z, Block, *this);
  }
}

BlockInt World::getBlock(int PosX, int PosY, int PosZ)
{
  Chunk* chunk = getChunkFromWorldPos(glm::ivec3(PosX, PosY, PosZ));
  
  if (PosY < chunkHeight && chunk != nullptr) {
    glm::ivec3 ChunkPos = getPosInChunk(PosX, PosY, PosZ);
    return chunk->getBlock(ChunkPos.x, ChunkPos.y, ChunkPos.z);
  }
  return 0;
}

std::array<Chunk*, Side::NumSides> World::getNearbyChunks(glm::ivec2 position) {
  std::array<Chunk*, Side::NumSides> nearbyChunks;
  
  nearbyChunks[Side::X_PLUS] = getChunkFromPos(glm::ivec2(position.x+1, position.y));
  nearbyChunks[Side::X_MIN] = getChunkFromPos(glm::ivec2(position.x-1, position.y));
  nearbyChunks[Side::Y_PLUS] = getChunkFromPos(glm::ivec2(position.x, position.y+1));
  nearbyChunks[Side::Y_MIN] = getChunkFromPos(glm::ivec2(position.x, position.y-1));

  return nearbyChunks;
}

void World::addRemoveChunks(Player& player, int renderRadius)
{
  //adding chunks
  glm::ivec2 PlayerChunkPosition(floor(player.position.x/float(chunkWidth)), floor(player.position.z/float(chunkWidth)));

  for (int i = PlayerChunkPosition.x-renderRadius; i < PlayerChunkPosition.x+renderRadius; i++) {
    for (int i2 = PlayerChunkPosition.y-renderRadius; i2 < PlayerChunkPosition.y+renderRadius; i2++) {
      if (getChunkFromPos(glm::ivec2(i, i2)) == nullptr) {
        chunks.push_back(Chunk()); //push back chunk if it doesn't exist within renderradius
        chunks.back().setPosition(glm::ivec2(i, i2));
      }
    }
  }

  //removing chunks
  for (unsigned int i = 0; i < chunks.size(); i++) {
    if (abs(chunks[i].getPosition().x - PlayerChunkPosition.x) > renderRadius || abs(chunks[i].getPosition().y - PlayerChunkPosition.y) > renderRadius) {
      chunks.erase(chunks.begin() + i);
    }
  }

}
