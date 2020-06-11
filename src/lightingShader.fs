#version 330 core
in vec3 fragPos;
in vec3 normal;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 fragColor;

void main() {
  // Ambient lighting
  vec3 ambient = light.ambient * material.ambient;

  // Diffuse lighting
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(light.position - fragPos);
  float diffuseStrength = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * (diffuseStrength * material.diffuse);

  // Specular lighting
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * material.specular);

  vec3 result = ambient + diffuse + specular;
  fragColor = vec4(result, 1.0);
}
