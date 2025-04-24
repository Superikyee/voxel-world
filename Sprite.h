#ifndef SPRITE_H
#define SPRITE_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Shader;
class Texture;

class Sprite
{
  public:
    Sprite();
    Sprite(Shader& shader);

    //create using shader if using default constructor
    void create(Shader& shader);
    void setTexture(Texture& _SpriteTexture);
    void setTexCoords(glm::vec4 Coords);
    void draw();

    void setPosition(glm::vec2 _Position) {position = _Position;}
    glm::vec2 getPosition() {return position;}
    void setSize(glm::vec2 _Size) {size = _Size;}
    glm::vec2 getSize() {return size;}
  protected:

  private:
    glm::vec2 position;
    glm::vec2 size;

    Texture* spriteTexture;
    float vertices[30];
    unsigned int VAO;
    unsigned int VBO;
    Shader* shader;
};

#endif // SPRITE_H
