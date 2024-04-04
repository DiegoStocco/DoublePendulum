#pragma once

#include <glm/vec2.hpp>

class DoublePendulum {
  public:
    DoublePendulum(glm::vec2 masses, glm::vec2 lengths, glm::vec2 start_angles, glm::vec2 start_velocities);

    void update(double deltaTime);

    void setAngles(glm::vec2 new_angles);
    glm::vec2 getAngles() const;

  private:
    glm::vec2 masses;
    glm::vec2 lengths;
    glm::vec2 angles;
    glm::vec2 ang_velocities;

};
