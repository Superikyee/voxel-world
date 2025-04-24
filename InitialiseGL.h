#ifndef INITIALISEGL_H_INCLUDED
#define INITIALISEGL_H_INCLUDED

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

///Creates a window with a specified size and title and returns the window pointer.<br>
///Also makes it current context
GLFWwindow* createGlfwWindow(const unsigned int Width, const unsigned int Height, const char* Title, void (*framebuffer_size_callback)(GLFWwindow*, int, int), void (*mouse_callback)(GLFWwindow*, double, double))
{
  GLFWwindow* window = glfwCreateWindow(Width, Height, Title, NULL, NULL);

  if (window == NULL) //error
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
  }
  glfwSetWindowPos(window, 600, 400);
  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  return window;
}

void loadGLAD()
{
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }
}

void setGLOptions(GLFWwindow* window)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  glEnable(GL_DEPTH_TEST);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  glCullFace(GL_FRONT);
  glFrontFace(GL_CCW);
}


#endif // INITIALISEGL_H_INCLUDED
