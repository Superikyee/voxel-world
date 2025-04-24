#include "Chunk.h"

#include <glm/vec3.hpp>

#include "Util.h"
#include "TextureAtlas.h"
#include "BlockVerticeGeneration.h"
#include "World.h"

Chunk::Chunk():
  needsVerticeUpdate(false), needsTerrainGeneration(true), terrainProcessed(false), ChunkBlocks(chunkWidth * chunkHeight * chunkWidth, 0), Empty(false)
{
  for (int i = 0; i < Side::NumSides; i++) {
    surroundingBlocks[i].resize(chunkWidth*chunkHeight);
  }
}

void Chunk::setBlockNoUpdateNearby(unsigned int X, unsigned int Y, unsigned int Z, BlockInt newBlock) {
  if (X >= chunkWidth) {
    std::cerr << "Chunk::setBlock: X over chunk dimensions\n";
    return;
  } else if (Y >=chunkHeight) {
    std::cerr << "Chunk::setBlock: Y over chunk dimensions\n";
    return;
  } else if (Z >= chunkWidth) {
    std::cerr << "Chunk::setBlock: Z over chunk dimensions\n";
    return;
  }

  ChunkBlocks[X * chunkHeight * chunkWidth + Y * chunkWidth + Z] = newBlock;

  needsVerticeUpdate = true;
}

void Chunk::setBlock(unsigned int X, unsigned int Y, unsigned int Z, BlockInt newBlock, World& world) {
  setBlockNoUpdateNearby(X, Y, Z, newBlock);

  //uptade surrounding chunks if we edited a block on chunk edge
  //(their vertices need to be updated too, since we may have exposed new faces)
  std::array<Chunk*, Side::NumSides> nearbyChunks = world.getNearbyChunks(position);

  if (X == chunkWidth - 1 && nearbyChunks[Side::X_PLUS] != nullptr) {
    nearbyChunks[Side::X_PLUS] -> needsVerticeUpdate = true;
  }
  if (X == 0 && nearbyChunks[Side::X_MIN] != nullptr) {
    nearbyChunks[Side::X_MIN] -> needsVerticeUpdate = true;
  }
  if (Z == chunkWidth - 1 && nearbyChunks[Side::Y_PLUS] != nullptr) {
    nearbyChunks[Side::Y_PLUS] -> needsVerticeUpdate = true;
  }
  if (Z == 0 && nearbyChunks[Side::Y_MIN] != nullptr) {
    nearbyChunks[Side::Y_MIN] -> needsVerticeUpdate = true;
  }
}

void Chunk::createSurroundingBlocks(World& world) {
  std::array<Chunk*, Side::NumSides> nearbyChunks = world.getNearbyChunks(position);

  for (int side = 0; side < Side::NumSides; side++) {
    for (unsigned int i = 0; i < chunkWidth; i++) {
      for (unsigned int j = 0; j < chunkHeight; j++) {

        //if a nearby chunk doesn't exist, then we store the surrounding blocks on that side as dirt
        //since if it doesn't exist, then we're at the edge of the world and we won't see those block faces anyway
        //so we store a non-transparent cube block which will make the vertices not generate -> faster rendering
        BlockInt returnBlock = Blocks::Dirt; 

        if (nearbyChunks[side] != nullptr) {
          //the actual x and z block coordinates in the surrounding chunks
          int x, z;

          if (side == Side::X_PLUS) {
            x = 0;
            z = i;
          } else if (side == Side::X_MIN) {
            x = chunkWidth - 1;
            z = i;
          } else if (side == Side::Y_PLUS) {
            x = i;
            z = 0;
          } else {
            x = i;
            z = chunkWidth - 1;
          }

          returnBlock = nearbyChunks[side]->getBlock(x, j, z);
        }

        surroundingBlocks[side][index2d(i, j, chunkWidth)] = returnBlock;
      }
    }
  }
}

std::vector<Vertice> Chunk::createVertices(TextureAtlas& atlas, BlockType::Type blocksToReturn)
{
  std::vector<Vertice> output;

  if (Empty) {
    return output;
  }

  for (int X = 0; X < chunkWidth; X++) {
    for (int Y = 0; Y < chunkHeight; Y++) {
      for (int Z = 0; Z < chunkWidth; Z++) {
        if (Data::blockType[getBlock(X, Y, Z)] == blocksToReturn) {
          std::vector<Vertice> currentVertices = getBlockVertices(*this, atlas, glm::ivec3(X, Y, Z));
          output.insert(output.end(), currentVertices.begin(), currentVertices.end());
        }
      }
    }
  }

  return output;
}


void Chunk::updateVertices(TextureAtlas& atlas)
{
  for (unsigned int i = 0; i < BlockType::NumTypes; i++) {
    chunkMesh.vertices[BlockType::Type(i)] = createVertices(atlas, BlockType::Type(i));
  }
}
