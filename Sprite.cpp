#include "Sprite.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"

Sprite::Sprite():
  position(0.0f), size(1.0f), spriteTexture(),
  vertices{
        //Pos             //Tex
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f
  },
  VAO(0), VBO(0), shader(nullptr)
{

}

Sprite::Sprite(Shader& shader):
  position(0.0f), size(1.0f), spriteTexture(),
  vertices{
        //Pos             //Tex
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f
  },
  VAO(0), VBO(0), shader(&shader)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

}

void Sprite::create(Shader& shader)
{
  this->shader = &shader;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

}

void Sprite::setTexture(Texture& _spriteTexture)
{
  spriteTexture = &_spriteTexture;
  size.x = spriteTexture->width;
  size.y = spriteTexture->height;
}

void Sprite::setTexCoords(glm::vec4 Coords)
{

  glm::vec4 nCoords(Coords.x/(float)spriteTexture->width,
                    (Coords.y-Coords.w)/(float)spriteTexture->height,
                    Coords.z/(float)spriteTexture->width,
                    Coords.w/(float)spriteTexture->height
  );
  vertices[3] = nCoords.x; vertices[4] = nCoords.y + nCoords.w;
  vertices[8] = nCoords.x + nCoords.z; vertices[9] = nCoords.y;
  vertices[13] = nCoords.x; vertices[14] = nCoords.y;

  vertices[18] = nCoords.x; vertices[19] = nCoords.y + nCoords.w;
  vertices[23] = nCoords.x + nCoords.z; vertices[24] = nCoords.y + nCoords.w;
  vertices[28] = nCoords.x + nCoords.z; vertices[29] = nCoords.y;


  vertices[5] = nCoords.z; vertices[6] = nCoords.w;
  vertices[11] =nCoords.w;
  vertices[20] = nCoords.z;
  vertices[25] = nCoords.z; vertices[26] = nCoords.w;
}

void Sprite::draw()
{
  //bind texture
  spriteTexture->bind();
  //apply transformations
  shader->use();

  glm::mat4 model(1.0f);

  model = glm::translate(model, glm::vec3(position, 0.0f));
  model = glm::scale(model, glm::vec3(size, 1.0f));

  shader->setMat4("model", model);

  // draw the actual sprite
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);

}
