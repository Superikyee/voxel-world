#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <readerwriterqueue/readerwriterqueue.h>

//Standard library
#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <thread>

//Project files
#include "Shader.h"
#include "InitialiseGL.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "Renderer.h"
#include "WorldShaders.h"
#include "Player.h"
#include "Chunk.h"
#include "World.h"
#include "Sprite.h"
#include "TerrainGeneration.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


//needed to access player in callback function
//init player at position, with default mouse x and y coords
Player player(glm::vec3(0.0f, 30.f, -3.f), SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f);


void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  player.mouseCallback(xpos, ypos);
}


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}


//Worker thread to generate terrain and chunk vertices
void chunkUpdater(moodycamel::ReaderWriterQueue<Chunk>& toProcess, moodycamel::ReaderWriterQueue<Chunk>& processed, std::atomic<bool>& stopUpdating) {
  Chunk chunk;

  //create another texture atlas which the thread can access safely
  TextureAtlas threadAtlas("img/spritesheet.png");

  while (!stopUpdating) {
    if (toProcess.try_dequeue(chunk)) {
      //prioritize updating terrain since terrain updates also have subsequent vertice updates
      if (chunk.needsTerrainGeneration) {
        generateTerrainForChunk(chunk);
        chunk.needsTerrainGeneration = false;
        chunk.terrainProcessed = false;
      } else {
        chunk.updateVertices(threadAtlas);
        chunk.needsVerticeUpdate = false;
      }

      //queue the processed chunk for the main thread
      processed.enqueue(chunk);
    }
    //sleep to avoid busy waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}


int main() {
  World world;
  const unsigned int renderRadius = 7;

  glfwInit();
  GLFWwindow* window = createGlfwWindow(SCR_WIDTH, SCR_HEIGHT, "Original concept do not steal", framebufferSizeCallback, mouseCallback);
  player.window = window;
  glfwSwapInterval(0); //disable vsync

  loadGLAD();
  setGLOptions(window);

  //openGL is fully loaded
  WorldShaders shaders;
  Renderer renderer;
  Shader spriteShader;
  spriteShader.generate("shader/SpriteShader.vert", "shader/SpriteShader.frag");
  TextureAtlas atlas("img/spritesheet.png");

  glClearColor(0.125f, 0.525f, 1.0f, 0.0f);

  Texture crosshairTex;
  crosshairTex.load("img/crosshair.png");
  Sprite crosshair(spriteShader);
  crosshair.setTexture(crosshairTex);
  crosshair.setPosition(glm::vec2(387, 287));

  moodycamel::ReaderWriterQueue<Chunk> chunksToProcess(20); //queue of chunks sent to worker thread
  moodycamel::ReaderWriterQueue<Chunk> processedChunks(20); //queue of processed chunks to main thread
  std::atomic<bool> stopUpdating = false;

  std::thread chunkUpdateThread(chunkUpdater, std::ref(chunksToProcess), std::ref(processedChunks), std::ref(stopUpdating));

  uint64_t cycles = 0; //for measuring fps
  glfwSetTime(0.0);

  Chunk processedChunk;

  //Main loop
  while (!glfwWindowShouldClose(window))
  {
    //set sine used to create water effect
    double waterSin = sin(glfwGetTime());
    shaders[ShaderType::WaterShader].use();
    shaders[ShaderType::WaterShader].setFloat("waterSin", waterSin);

    //receive chunks with generated terrain and vertices
    while (processedChunks.try_dequeue(processedChunk)) {
      Chunk* toReplace = world.getChunkFromPos(processedChunk.getPosition());

      if (toReplace != nullptr) {
        *toReplace = processedChunk;
      }
    }

    //find chunks without terrain and send them to worker thread
    bool allTerrainDone = true;

    for (unsigned int i = 0; i < world.chunks.size(); i++) {
      //if we're not processing terrain of a chunk already, do it
      if (world.chunks[i].terrainProcessed) {
        allTerrainDone = false;
      } else if (world.chunks[i].needsTerrainGeneration)  {
        allTerrainDone = false;
        //create a copy of the chunk to process
        Chunk chunkCopy = world.chunks[i];

        world.chunks[i].terrainProcessed = true;
        world.chunks[i].needsTerrainGeneration = false;
        chunksToProcess.enqueue(chunkCopy);
      }
    }

    //if the terrain is fully generated, we can move on to vertices
    if (allTerrainDone) {
      for (unsigned int i = 0; i < world.chunks.size(); i++) {
        if (world.chunks[i].needsVerticeUpdate)  {
          world.chunks[i].createSurroundingBlocks(world);
          // Create a copy of the chunk to process
          Chunk chunkCopy = world.chunks[i];
          world.chunks[i].needsVerticeUpdate = false;

          chunksToProcess.enqueue(chunkCopy);
        }
      }
    }

    player.processMovement(cycles);
    player.changeBlocks(world);
    world.addRemoveChunks(player, renderRadius);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw world!
    atlas.getTexture().bind();
    renderer.setMatrices(shaders, player, SCR_WIDTH, SCR_HEIGHT);
    renderer.drawChunks(world, shaders);

    //draw crosshair
    spriteShader.use();
    glm::mat4 projection(1.0f);
    projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.0f, 2000.f);
    spriteShader.setMat4("projection", projection);
    crosshair.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();

    cycles++;
  }

  std::cout << "Average FPS: " << cycles / glfwGetTime() << '\n';
  glfwDestroyWindow(window);

  stopUpdating = true;
  chunkUpdateThread.join();

  glfwTerminate();

  return 0;
}

