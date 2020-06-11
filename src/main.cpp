#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// Timing
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Light
const float LIGHT_RADIUS = 2.0f;
glm::vec3 lightPos(1.2f, 1.0f, LIGHT_RADIUS);
float ambientStrength = 0.1f;
float specularStrength = 0.5f;
int shininess = 32;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create a GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // Mouse input
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Configure global opengl state
  glEnable(GL_DEPTH_TEST);

  // Create shader program
  Shader lightingShader("../src/shader.vs", "../src/lightingShader.fs");
  Shader lightCubeShader("../src/shader.vs", "../src/lightCubeShader.fs");

  // Data
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };


  // Cube VAO setup
  unsigned int cubeVBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);

  // VAO
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // normal attributes
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Allowed to unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);


  // Light VAO setup
  unsigned int lightVBO, lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &lightVBO);

  // VAO
  glBindVertexArray(lightVAO);
  // Possible to use a single VBO for both the VAO's, clearer this way
  glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Allowed to unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    // Time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame -  lastFrame;
    lastFrame = currentFrame;

    // input
    processInput(window);

    // rendering
    glClearColor(0.05f, 0.075f, 0.075f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set lightingShader uniforms, needed in loop since
    // the view position is updated
    lightingShader.use();
    lightingShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
    lightingShader.setVec3("material.diffuse", 0.0f, 0.509f, 0.509f);
    lightingShader.setVec3("material.specular", 0.501f, 0.501f, 0.501f);
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("light.position", lightPos);
    lightingShader.setVec3("light.ambient", glm::vec3(1.0f));
    lightingShader.setVec3("light.diffuse", glm::vec3(1.0f));
    lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("viewPos", camera.getPosition());

    // Projection
    glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH/ (float)SCR_HEIGHT, 0.1f, 100.0f);
    lightingShader.setMat4("projection", projection);

    // View
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader.setMat4("view", view);

    // Render
    glBindVertexArray(cubeVAO);
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // lightCube
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightCubeShader.setMat4("model", model);

    // Draw the lightCube
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Deallocate resources when no longer necessary
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &lightVBO);

  glfwTerminate();

  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

  // If entering the window for the first time
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.processMouseScroll(yoffset);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  // Movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);
  }
}
