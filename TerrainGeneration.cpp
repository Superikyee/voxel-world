#include "TerrainGeneration.h"

#include <glm/vec2.hpp>
#include <FastNoiseLite/FastNoiseLite.h>

#include <random>
#include <ctime>
#include <cmath>
#include <iostream>

#include "Chunk.h"
#include "World.h"

void generateTerrainForChunk(Chunk& chungus)
{
  int WaterHeight = 20;
  glm::ivec2 chunkPos = chungus.getPosition();
  FastNoiseLite noise;

  noise.SetFrequency(0.01f);
  noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

  for (int i = 0; i < chunkWidth; i++) {
    for (int i2 = 0; i2 < chunkWidth; i2++) {

      glm::ivec2 BlockPos(chunkPos.x*chunkWidth+i, chunkPos.y * chunkWidth+i2);
      int Height = (noise.GetNoise(float(BlockPos.x), float(BlockPos.y))) * 10 + 20;
      if (Height < 1) {Height = 1;}
      if (Height >= chunkHeight) {Height = chunkHeight-1;}

      for (int i3 = 0; i3 < Height || i3 < WaterHeight; i3++) {
        BlockInt CurrentBlock;

        if (i3 < Height) {
          
          if (i3 < WaterHeight-2) {
            if (rand() % 2 == 0) {
              CurrentBlock = Blocks::Clay;
            } else {
              CurrentBlock = Blocks::Dirt;
            }
          } else if (i3 < WaterHeight + 2) {
            CurrentBlock = Blocks::Sand;
          } else if (i3 == Height-1) {
            CurrentBlock = Blocks::Grass;
          } else {
            CurrentBlock = Blocks::Dirt;
          }

        } else if (i3 < WaterHeight) {
          CurrentBlock = Blocks::Water;
        } else {
          CurrentBlock = Blocks::Air;
        }

        chungus.setBlockNoUpdateNearby(i, i3, i2, CurrentBlock);
      }
    }
  }
}
