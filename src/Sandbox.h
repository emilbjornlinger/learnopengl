#ifndef SANDBOX_H
#define SANDBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"

// Settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// Camera, this is horrible :(
static Camera m_camera(glm::vec3(0.0f, 1.0f, 3.0f));
static float m_lastX = SCR_WIDTH / 2.0f;
static float m_lastY = SCR_HEIGHT / 2.0f;
static bool m_firstMouse = true;

class Sandbox {
private:
  GLFWwindow* m_window;
  Shader* m_shader;
  Shader* m_groundShader;

  // Timing
  float m_deltaTime = 0.0f; // Time between current frame and last frame
  float m_lastFrame = 0.0f; // Time of last frame

  unsigned int m_cubeVAO, m_cubeVBO, m_groundVAO, m_groundVBO, m_groundEBO;
  unsigned int m_texture1, m_texture2, m_groundTexture;

  bool m_statusOK = true;

  // Physics variables

  // Initialising functions
  void initGLFW();
  void initShaders();
  void initVAO();
  void initTextures();

  // Callback functions
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

  void processInput(GLFWwindow* window);
  glm::vec3 getPosition();
public:
  Sandbox();
  ~Sandbox();
  bool getStatus() { return m_statusOK; }
  void run();
};

#endif // SANDBOX_H
