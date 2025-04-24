#ifndef BLOCKVERTICEGENERATION_H_INCLUDED
#define BLOCKVERTICEGENERATION_H_INCLUDED

#include <glm/vec3.hpp>

#include <iostream>
#include <stdio.h>
#include <array>
#include <vector>

#include "Chunk.h"
#include "VariableTypes.h"
#include "Vertice.h"
#include "TextureAtlas.h"
#include "World.h"


//Vertice positions for a cube: 6 faces, two triangles per face,
//three vertices per triangle, three coordinates per vertice
constexpr std::array<std::array<float, 18>, 6> verticePositions =
{{
  {
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f
  },
  {
    -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f
  },
  {
    -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f
  },
  {
    0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f
  },
  {
    0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f
  },
  {
    -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f
  }
}};


//Texture coordinates for a cube
constexpr std::array<std::array<unsigned int, 12>, 6> texCoords = //unsigned int because it represents array indexes
{{
  {
    2, 4, 0, 6, 8, 10,
    3, 5, 1, 7, 9, 11
  },
  {
    2, 0, 4, 8, 6, 10,
    3, 1, 5, 9, 7, 11
  },
  {
    0, 2, 4, 6, 8, 10,
    1, 3, 5, 7, 9, 11
  },
  {
    2, 0, 4, 8, 6, 10,
    3, 1, 5, 9, 7, 11
  },
  {
    2, 0, 4, 8, 6, 10,
    3, 1, 5, 9, 7, 11
  },
  {
    0, 2, 4, 6, 8, 10,
    1, 3, 5, 7, 9, 11
  }
}};



//Given a block, get the surrounding block values
inline std::array<BlockInt, BlockFace::NumFaces> getNearbyBlocks(Chunk& chunk, glm::ivec3 pos) {
  std::array<BlockInt, BlockFace::NumFaces> output;

  //X
  if (pos.x == chunkWidth-1) {
    output[BlockFace::X_PLUS] = chunk.surroundingBlocks[Side::X_PLUS][pos.y*chunkWidth+pos.z];
  } else {
    output[BlockFace::X_PLUS] = chunk.getBlock(pos.x+1, pos.y, pos.z); //if block is not on edge, it will just check for next block in current chunk
  }

  if (pos.x == 0) {
    output[BlockFace::X_MIN] = chunk.surroundingBlocks[Side::X_MIN][pos.y*chunkWidth+pos.z];
  } else {
    output[BlockFace::X_MIN] = chunk.getBlock(pos.x-1, pos.y, pos.z);
  }

  //Y
  if (pos.y == chunkHeight-1) {
    output[BlockFace::TOP] = Blocks::Air;
  } else {
    output[BlockFace::TOP] = chunk.getBlock(pos.x, pos.y+1, pos.z);
  }

  if (pos.y == 0) {
    output[BlockFace::BOTTOM] = Blocks::Air;
  } else {
    output[BlockFace::BOTTOM] = chunk.getBlock(pos.x, pos.y-1, pos.z);
  }

  //Z
  if (pos.z == chunkWidth-1) {
    output[BlockFace::Z_PLUS] = chunk.surroundingBlocks[Side::Y_PLUS][pos.y*chunkWidth+pos.x];
  } else {
    output[BlockFace::Z_PLUS] = chunk.getBlock(pos.x, pos.y, pos.z+1);
  }

  if (pos.z == 0) {
    output[BlockFace::Z_MIN] = chunk.surroundingBlocks[Side::Y_MIN][pos.y*chunkWidth+pos.x];
  } else {
    output[BlockFace::Z_MIN] = chunk.getBlock(pos.x, pos.y, pos.z-1);
  }

  return output;
}

inline void addFace(std::vector<Vertice>& output, TextureAtlas& atlas, Chunk& chunk, glm::ivec3 pos, BlockFace::Type face) //adds a block face using coords from the array
{
  std::vector<float> texC = atlas.getFaceTexCoords(chunk.getBlock(pos.x, pos.y, pos.z), face);
  for (unsigned int i = 0; i < 6; i++) {
    output.push_back(Vertice
    (
      verticePositions[face][i] + pos.x + chunk.getPosition().x * chunkWidth,  verticePositions[face][i+6] + pos.y,
      verticePositions[face][i+12] + pos.z + chunk.getPosition().y * chunkWidth,
      texC[texCoords[face][i]], texC[texCoords[face][i+6]])
    );
  }
}

inline std::vector<Vertice> getBlockVertices(Chunk& chunk, TextureAtlas& atlas, glm::ivec3 pos)
{
  std::vector<Vertice> output;
  std::array<BlockInt, BlockFace::NumFaces> nearbyBlocks = getNearbyBlocks(chunk, pos);
  BlockInt CurrentBlock = chunk.getBlock(pos.x, pos.y, pos.z);
  BlockType::Type currentType = Data::blockType[CurrentBlock];

  switch (currentType) {
    case BlockType::Cube:
      for (int i = 0; i < BlockFace::NumFaces; i++) {
        if (Data::isTransparent[nearbyBlocks[BlockFace::Type(i)]]) {
          addFace(output, atlas, chunk, pos, BlockFace::Type(i));
        }
      }
      break;
    case BlockType::Water:
      for (int i = 0; i < BlockFace::NumFaces; i++) {
        if (nearbyBlocks[BlockFace::Type(i)] != Blocks::Water && (BlockFace::Type(i)  == BlockFace::TOP || Data::isTransparent[nearbyBlocks[BlockFace::Type(i)]])) {
          addFace(output, atlas, chunk, pos, BlockFace::Type(i));
        }
      }
    default:
      break;
  }

  return output;
}

#endif // BLOCKVERTICEGENERATION_H_INCLUDED
