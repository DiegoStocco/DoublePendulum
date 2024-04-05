#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/vec2.hpp>

#include <vertex_buffer.h>
#include <vertex_array.h>
#include <index_buffer.h>
#include <shader.h>
#include <window.h>

class DoublePendulum {
  public:
    DoublePendulum(glm::vec2 masses, glm::vec2 lengths, glm::vec2 start_angles, glm::vec2 start_velocities);
    ~DoublePendulum();

    void update(double deltaTime);
    void render(JAGE::Window* window);

    void setAngles(glm::vec2 new_angles);
    glm::vec2 getAngles() const;

  private:
    glm::vec2 masses;
    glm::vec2 lengths;
    glm::vec2 angles;
    glm::vec2 ang_velocities;

    JAGE::VertexBuffer* vb;
    JAGE::VertexArray* va;
    JAGE::IndexBuffer* ib;
    JAGE::Shader* colored_trigs;

    glm::mat4 model_matrices[2];

};
