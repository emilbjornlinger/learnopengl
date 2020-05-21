#ifndef SANDBOX_H
#define SANDBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"

const float GRAVITY = 4.4f;

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

  // Physics variables, better in separate class really
  glm::vec3 m_vel;
  glm::vec3 m_pos;
  bool m_active;

  // Physics functions
  void throwNewCube();
  void updateCube(float dt);
  void checkCollision();

  // Initialising functions
  void initGLFW();
  void initShaders();
  void initVAO();
  void initTextures();

  // Callback functions
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  // User pointer
  static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  void mouse_callback(double xpos, double ypos);
  static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
  void mouse_button_callback(int button, int action, int mods);

  void processInput(GLFWwindow* window);
public:
  Sandbox();
  ~Sandbox();
  bool getStatus() { return m_statusOK; }
  void run();
};

#endif // SANDBOX_H
