#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 color;

uniform mat4 MPV;

out vec3 COLOR;

void main() {
  gl_Position = MPV * vec4(vertex_position, 1);
  COLOR = color;
}
