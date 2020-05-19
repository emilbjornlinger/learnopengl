#ifndef SANDBOX_H
#define SANDBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"

class Sandbox {
private:
  GLFWwindow* m_window;
  Shader* m_shader;
  Shader* m_groundShader;

  // Settings
  const unsigned int SCR_WIDTH = 1920;
  const unsigned int SCR_HEIGHT = 1080;

  // Camera
  Camera m_camera;
  float m_lastX = SCR_WIDTH / 2.0f;
  float m_lastY = SCR_HEIGHT / 2.0f;
  bool m_firstMouse = true;

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

  // called from callback through user pointer
  void mouse_callback(double xpos, double ypos);

  void processInput(GLFWwindow* window);
  glm::vec3 getPosition();
public:
  Sandbox();
  ~Sandbox();
  bool getStatus() { return m_statusOK; }
  void run();
};

#endif // SANDBOX_H
