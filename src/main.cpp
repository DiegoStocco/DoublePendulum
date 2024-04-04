#include "double_pendulum.hpp"
#include <glm/ext/vector_float2.hpp>
#include <iostream>
#include <window.h>
#include <errors.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

JAGE::Window* init()
{
  // Initialize the library
  if (!glfwInit())
    std::cerr << "Error in initializing GLFW\n";

  JAGE::enable_glfw_errors();

  JAGE::Window *window = new JAGE::Window("Field", 900, 700);
  window->Bind();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Init GLEW to import opengl functions
  if(glewInit() != GLEW_OK)
    std::cerr << "Error in GLEW initialization\n";

  JAGE::enable_gl_errors();

  // Print openGL version
  std::cout << glGetString(GL_VERSION) << "\n";

  return window;
}

int main(int argc, char* argv[]) {
  JAGE::Window* window = init();

  glm::vec2 masses({1.0, 1.0});
  glm::vec2 lengths({1.0, 1.0});
  glm::vec2 angles({1.5, 1.5});
  glm::vec2 angular_velocity({0, 0});
  DoublePendulum pendulum(masses, lengths, angles, angular_velocity);

  double time = glfwGetTime();
  double oldtime = glfwGetTime();
  double deltaTime;
  while(!window->shouldClose()) {
    deltaTime = time - oldtime;

    // Update pendulum's position and velocity
    pendulum.update(deltaTime);

    // TODO: render the pendulum, for now you can have the angles printed on stdout
    std::cout << pendulum.getAngles().x << " " << pendulum.getAngles().y << std::endl;
    
    oldtime = time;
    time = glfwGetTime();

    glfwPollEvents();
  }

  delete window;
}
