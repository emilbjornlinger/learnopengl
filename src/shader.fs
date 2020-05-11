#version 330 core
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragColor;

void main() {
  fragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), 0.5f);
}
