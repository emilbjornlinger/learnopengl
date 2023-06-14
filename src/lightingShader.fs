#version 330 core
in vec3 fragPos;
in vec3 normal;
in vec2 TexCoords;

/* Several lights */
struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
}

/* Individual lighting */

// struct Material {
//   sampler2D diffuse;
//   sampler2D specular;
//   sampler2D emission;
//   float shininess;
// };
//
// struct Light {
//   vec3 position;
//   vec3 direction;
//   float cutOff;
//   float outerCutOff;
//
//   vec3 ambient;
//   vec3 diffuse;
//   vec3 specular;
//
//   float constant;
//   float linear;
//   float quadratic;
// };
//
// uniform Material material;
// uniform Light light;
// uniform vec3 viewPos;

out vec4 fragColor;


void main() {
  /* Several lights */

  /* Individual lighting */

  // // Ambient lighting
  // vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  //
  // // Diffuse lighting
  // vec3 norm = normalize(normal);
  // vec3 lightDir = normalize(light.position - fragPos);
  // //vec3 lightDir = normalize(-light.direction);
  // float diffuseStrength = max(dot(norm, lightDir), 0.0);
  // vec3 diffuse = light.diffuse * diffuseStrength * texture(material.diffuse, TexCoords).rgb;
  //
  // // Specular lighting
  // vec3 viewDir = normalize(viewPos - fragPos);
  // vec3 reflectDir = reflect(-lightDir, norm);
  // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  // vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
  //
  // // Spotlight
  // float theta = dot(lightDir, normalize(-light.direction));
  // float epsilon = light.cutOff - light.outerCutOff;
  // float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
  // diffuse *= intensity;
  // specular *= intensity;
  //
  // // Attenuation
  // float distance = length(light.position - fragPos);
  // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  // //ambient *= attenuation;
  // diffuse *= attenuation * intensity;
  // specular *= attenuation * intensity;
  //
  // // // Emission map
  // // vec3 emission = vec3(0.0);
  // // if (texture(material.specular, TexCoords).r == 0.0) {
  // //   emission = texture(material.emission, TexCoords).rgb;
  // // }
  //
  // fragColor = vec4(ambient + diffuse + specular, 1.0);
}
