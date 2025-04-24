#include "Renderer.h"

#include <glm/mat4x4.hpp>

#include <iostream>

#include "World.h"
#include "Shader.h"
#include "WorldShaders.h"
#include "Player.h"
#include "Chunk.h"

Renderer::Renderer()
{
  for (unsigned int i = 0; i < BlockType::NumTypes; i++) {
    glGenVertexArrays(1, &VAO[i]);
    glBindVertexArray(VAO[i]);
    glGenBuffers(1, &VBO[i]);
    //bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes

    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }
}

void Renderer::drawChunks(World& world, WorldShaders& shaders)
{
  glDisable(GL_CULL_FACE);

  for (unsigned int i = 0; i < BlockType::NumTypes; i++) {

    bool canUseCullFace = true;
    for (BlockInt i2 = 0; i2 < Data::numBlocks; i2++) {
      if (Data::blockType[i2] == BlockType::Type(i) && !Data::canUseCullFace[i2]) {
        canUseCullFace = false; //if there are any blocks in this blocktype that cant use cull face, itll be disabled
        break;
      }
    }

    if (canUseCullFace) {
      glEnable(GL_CULL_FACE);
    } else {
      glDisable(GL_CULL_FACE);
    }

    shaders[Data::shaderType[BlockType::Type(i)]].use();
    for (unsigned int i2 = 0; i2 < world.chunks.size(); i2++) {
      std::vector<Vertice> *currentArray = &world.chunks[i2].chunkMesh.vertices[BlockType::Type(i)];

      glBindVertexArray(VAO[i]);
      glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
      glBufferData(GL_ARRAY_BUFFER, currentArray->size() * sizeof(Vertice), currentArray->data(), GL_STATIC_DRAW);
      glDrawArrays(GL_TRIANGLES, 0, currentArray->size());
    }
  }
}

void Renderer::setMatrices(WorldShaders& shaders, Player& player, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT)
{
  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);
  glm::mat4 projection(1.0f);

  view = player.getViewMatrix();

  projection = glm::perspective(glm::radians(player.zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 2000.0f);

  for (unsigned int i = 0; i < ShaderType::NumTypes; i++) {
    shaders[ShaderType::Type(i)].use();

    shaders[ShaderType::Type(i)].setMat4("model", model);
    shaders[ShaderType::Type(i)].setMat4("view", view);
    shaders[ShaderType::Type(i)].setMat4("projection", projection);
  }
}
