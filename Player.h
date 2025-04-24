#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class World;
typedef struct GLFWwindow GLFWwindow;

class Player
{
  public:
    Player(glm::vec3 position, float lastX, float lastY);

    void changeBlocks(World& world);
    void processMovement(float cycles);

    glm::mat4 getViewMatrix() {
      return glm::lookAt(position, position + Front, Up);
    }

    void mouseCallback(double xpos, double ypos);
    
    GLFWwindow* window;
    float blockChangeCooldown;

    //camera Attributes
    glm::vec3 position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    
    float yaw;
    float pitch;
    
    float MouseSensitivity;
    float zoom;

    bool firstMouse;
    float lastX;
    float lastY;

    float lastTime;
    float deltaTime;
  protected:

  private:

    //calculates the front vector from the Euler Angles
    void updateCameraVectors() {
      glm::vec3 front;
      front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      front.y = sin(glm::radians(pitch));
      front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      Front = glm::normalize(front);
      
      Right = glm::normalize(glm::cross(Front, glm::vec3(0.f, 1.f, 0.f)));
      Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif // PLAYER_H
