#version 330 core
in vec3 ourColor;
in vec3 colPos;

out vec4 fragColor;

void main() {
  fragColor = vec4(colPos, 1.0f);
}
