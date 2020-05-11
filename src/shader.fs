#version 330 core
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixFloat;

out vec4 fragColor;

void main() {
  fragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0f-TexCoord.x, TexCoord.y)), mixFloat);
}
