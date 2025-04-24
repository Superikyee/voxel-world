#include "Player.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <iostream>
#include <cmath>

#include "World.h"
#include "VariableTypes.h"

Player::Player(glm::vec3 position, float lastX, float lastY):
  blockChangeCooldown(0.f), position(position), yaw(0.f), pitch(0.f), MouseSensitivity(0.2f), zoom(45.f), firstMouse(true), lastX(lastX), lastY(lastY), lastTime(0.f)
{
  updateCameraVectors();
}


void Player::mouseCallback(double xpos, double ypos) {
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (pitch > 89.0f) {
    pitch = 89.0f;
  }
  if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}

void Player::processMovement(float Cycles) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float newTime = glfwGetTime();
  deltaTime = newTime - lastTime;
  float cameraSpeed = deltaTime * 30.f;
  lastTime = newTime;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    position += glm::normalize(glm::vec3(Front.x, 0.f, Front.z)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    position -= glm::normalize(glm::vec3(Front.x, 0.f, Front.z)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    position -= glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    position += glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
    position.y += cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
    position.y -= cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
    position += cameraSpeed * Front;
  }
}

void Player::changeBlocks(World& world)
{
  bool Left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
  bool Right = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

  if ((Left || Right) && blockChangeCooldown <= 0) {
    glm::vec3 pos = position;
    glm::vec3 dir = Front;

    //start stepping ray
    for (unsigned int i = 0; i < 2000; i++) {
      pos += dir * 0.01f;
      glm::ivec3 PosInt(round(pos.x), round(pos.y), round(pos.z)); //otherwise it will round down

      if (world.getBlock(PosInt.x, PosInt.y, PosInt.z) != Blocks::Air && world.getBlock(PosInt.x, PosInt.y, PosInt.z) != Blocks::Water) {
        if (Right) {
          world.setBlock(0, PosInt.x, PosInt.y, PosInt.z); //remove block
        } else {
          while (world.getBlock(PosInt.x, PosInt.y, PosInt.z) != Blocks::Air && world.getBlock(PosInt.x, PosInt.y, PosInt.z) != Blocks::Water) {
            pos -= dir * 0.01f; //if placing a block, step backwards until we're in a different block
            PosInt = glm::ivec3(round(pos.x), round(pos.y), round(pos.z));
          }
          world.setBlock(Blocks::Grass, PosInt.x, PosInt.y, PosInt.z);
        }
        blockChangeCooldown = 0.5f;
        break;
      }
    }
  }
  if (blockChangeCooldown > 0) {
    blockChangeCooldown -= deltaTime;
  }
}
