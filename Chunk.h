#ifndef CHUNK_H
#define CHUNK_H

#include <glm/vec2.hpp>

#include <iostream>
#include <vector>
#include <array>
#include <atomic>

#include "Vertice.h"
#include "VariableTypes.h"
#include "Mesh.h"
#include "World.h"

class TextureAtlas;
class World;

const int chunkWidth = 16;
const int chunkHeight = 50;

class Chunk
{
  public:
    Chunk();

    //generate vertices needed to render chunk
    void updateVertices(TextureAtlas& atlas);

    void setBlock(unsigned int X, unsigned int Y, unsigned int Z, BlockInt newBlock, World& world);
    void setBlockNoUpdateNearby(unsigned int X, unsigned int Y, unsigned int Z, BlockInt newBlock);
    BlockInt getBlock(unsigned int X, unsigned int Y, unsigned int Z) const
    {
      return ChunkBlocks[X * chunkHeight * chunkWidth + Y * chunkWidth + Z];
    }

    std::vector<BlockInt>& getBlockVector() {return ChunkBlocks;}
    void setPosition(glm::ivec2 Pos) {position = Pos;}
    glm::ivec2 getPosition() const {return position;}

    bool needsVerticeUpdate; //whether current vertices reflect current blocks in chunk
    bool needsTerrainGeneration;
    bool terrainProcessed; //whether this chunk is being processed by the worker thread
    Mesh chunkMesh;
    std::array<std::vector<BlockInt>, Side::NumSides> surroundingBlocks;
    void createSurroundingBlocks(World& world);
  protected:

  private:
    std::vector<Vertice> createVertices(TextureAtlas& atlas, BlockType::Type blocksToReturn);

    std::vector<BlockInt> ChunkBlocks;
    glm::ivec2 position;
    bool Empty;
};

#endif // CHUNK_H
