#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 color;


void main() {
  // Multiplication from right to left
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  vec3 fragPos = vec3(model * vec4(aPos, 1.0));
  vec3 normal = mat3(transpose(inverse(model))) * aNormal;

  // Ambient lighting
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse lighting
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - fragPos);
  float diffuseStrength = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diffuseStrength * lightColor;

  // Specular lighting
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0);
}
