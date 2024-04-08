#include "double_pendulum.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <window.h>
#include <errors.h>
#include <renderer.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

JAGE::Window* init()
{
  // Initialize the library
  if (!glfwInit())
    std::cerr << "Error in initializing GLFW\n";

  JAGE::enable_glfw_errors();

  JAGE::Window *window = new JAGE::Window("Double Pendulum", 900, 700);
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
  glm::vec2 lengths({2.2, 2.0});
  glm::vec2 angles({glm::radians(90.0), glm::radians(180.0)});
  glm::vec2 angular_velocity({0, 0});
  DoublePendulum pendulum(masses, lengths, angles, angular_velocity);

  JAGE::Renderer renderer;

  double time = glfwGetTime();
  double oldtime = glfwGetTime();
  double deltaTime;
  while(!window->shouldClose()) {
    deltaTime = time - oldtime;

    // Clean screen
    renderer.Clear();

    // Update pendulum's position and velocity
    pendulum.update(deltaTime);

    // Render the pendulum
    pendulum.render(window);

    // Update screen
    glfwSwapBuffers(window->getWindow());
    
    oldtime = time;
    time = glfwGetTime();

    glfwPollEvents();
  }

  delete window;
}
