#include "double_pendulum.hpp"
#include "constants.hpp"
#include "index_buffer.h"
#include "shader.h"
#include "renderer.h"
#include "glm/ext/matrix_transform.hpp"

using Constants::g;

/*
Description:
  Create an instance on a double pendulum.
Arguments:
  glm::vec2 masses: A vector containing the masses in kg
                    of the two pendulums.
  glm::vec2 lengths: A vector containing the lengths in meters
                     of the rods where the masses are
                     attached
  glm::vec2 start_angles: The initial angles of the
                          two pendulums. The angles are
                          in radians, the zero is set on
                          the vertical, under the pendulum.
                          The angles get positive in 
                          anticlockwise direction.
  glm::vec2 start_velocities: The initial velocities of
                              the pendulums in rad/s.
                              Anticlockwise is positive.
*/

DoublePendulum::DoublePendulum(glm::vec2 masses, glm::vec2 lengths, glm::vec2 start_angles, glm::vec2 start_velocities)
                 : masses(masses), lengths(lengths), angles(start_angles), ang_velocities(start_velocities)
{
  float positions_and_colors[] = {
  //x,     y,   z,   r,   g,   b
    -0.3,  0.5, 0.0, 0.2, 0.1, 0.8,
     0.3,  0.5, 0.0, 0.2, 0.1, 0.8,
     0.3, -0.5, 0.0, 0.2, 0.1, 0.8,
    -0.3, -0.5, 0.0, 0.2, 0.1, 0.8,
  };

  vb = new JAGE::VertexBuffer(positions_and_colors, sizeof(positions_and_colors));

  JAGE::VertexBufferLayout layout;
  layout.Push<float>(3);
  layout.Push<float>(3);

  va = new JAGE::VertexArray();
  va->AddBuffer(*vb, layout);
  
  unsigned int indicies[] = {
    0, 1, 2,
    2, 3, 0,
  };

  ib = new JAGE::IndexBuffer(indicies, sizeof(indicies));

  colored_trigs = new JAGE::Shader({{"shaders/apply_matrix.vert", GL_VERTEX_SHADER}, {"shaders/colored_trigs.frag", GL_FRAGMENT_SHADER}});
};

DoublePendulum::~DoublePendulum() {
  delete vb;
  delete va;
  delete ib;
  delete colored_trigs;
}

/*
Description:
  Update the pendulum position and velocity using
  a formula found with the lagrangian of the
  double pendulum.
  Does the formula work? I have no idea, we'll see
  if i implement the graphics things.

Arguments:
  double deltaTime: the amount of time in seconds
                    passed since the last update.
*/
void DoublePendulum::update(double deltaTime) {
  double a = (masses[0] + masses[1]) * std::pow(lengths[0], 2.0);
  double b = masses[1] * lengths[0] * lengths[1] * std::cos(angles[0] - angles[1]);
  double c = masses[1] * lengths[0] * lengths[1] * ang_velocities[0] * ang_velocities[1] * std::sin(angles[0] - angles[1])
             + (masses[0] + masses[1]) * g * lengths[0] * std::sin(angles[0])
             + masses[1] * lengths[0] * lengths[1] * ang_velocities[1] * (ang_velocities[0] - ang_velocities[1]) * std::sin(angles[0] - angles[1]);
  double ap = masses[1] * lengths[0] * lengths[1] * std::cos(angles[0] - angles[1]);
  double bp = masses[1] * std::pow(lengths[1], 2.0);
  double cp = masses[1] * lengths[0] * lengths[1] * ang_velocities[0] * ang_velocities[1] * std::sin(angles[0] - angles[1])
              + masses[1] * g * lengths[1] * std::sin(angles[1])
              + masses[1] * lengths[0] * lengths[1] * ang_velocities[0] * (ang_velocities[0] - ang_velocities[1]) * std::sin(angles[0] - angles[1]);

  ang_velocities[0] += deltaTime * (bp*c - b*cp)/(a*bp - ap*b);
  angles[0] += deltaTime * ang_velocities[0];
  angles[0] = std::fmod(angles[0], glm::radians(360.0));

  ang_velocities[1] += deltaTime * (a*cp - ap*c)/(a*bp - ap*b);
  angles[1] += deltaTime * ang_velocities[1];
  angles[1] = std::fmod(angles[1], glm::radians(360.0));
}

void DoublePendulum::render(JAGE::Window* window) {
  UpdateMatrices();

  JAGE::Renderer renderer;
  window->Bind();
  renderer.Clear();
  for(int i : {0, 1}) {
    colored_trigs->SetUniformMatrix4("MPV", model_matrices + i);
    renderer.Draw(*va, *ib, *colored_trigs);
  }
  glfwSwapBuffers(window->getWindow());
}

/*
Description:
  Set new angles to the pendulums.
Arguments:
  glm::vec2 new_angles: The new angles of the
                        pendulums, in radians, 
                        the zero is set on the
                        vertical, under the pendulum.
                        The angles get positive in 
                        anticlockwise direction. 
*/
void DoublePendulum::setAngles(glm::vec2 new_angles) {
  angles = new_angles;
}

/*
Description:
  Get the pendulums' angles.
Return value:
  The angles of the
  pendulums, in radians, 
  the zero is set on the
  vertical, under the pendulum.
  The angles get positive in 
  anticlockwise direction. 
*/
glm::vec2 DoublePendulum::getAngles() const {
  return angles;
}

void DoublePendulum::UpdateMatrices() {
  // This value should be somewhere else
  float scale_coeff = 0.2f;

  // Update matrix 1
  {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale_coeff, lengths[0]*scale_coeff, 1.0f));
    glm::mat4 move_top_to_origin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -scale_coeff * lengths[0]/2.0, 0.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angles[0], glm::vec3(0,0,1));

    model_matrices[0] = rotate * move_top_to_origin * scale;
  }

  // Update matrix 2
  {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale_coeff, lengths[1]*scale_coeff, 1.0f));
    glm::mat4 move_top_to_origin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -scale_coeff * lengths[1]/2.0, 0.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angles[1], glm::vec3(0,0,1));

    glm::vec3 pendulum1_end = model_matrices[0] * glm::vec4(0, -0.5, 0, 1);
    glm::mat4 move_top_to_pendulm1 = glm::translate(glm::mat4(1.0f), pendulum1_end);

    model_matrices[1] = move_top_to_pendulm1 * rotate * move_top_to_origin * scale;
  }
}
