#version 330 core
layout(location = 0) out vec4 color;
in vec3 COLOR;

void main() {
  color = vec4(COLOR, 1);
}
