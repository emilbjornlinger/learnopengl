#include "Sandbox.h"

#include <iostream>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Sandbox::Sandbox() :
  m_camera(glm::vec3(0.0f, 1.0f, 3.0f)),
  m_vel(glm::vec3(0.0f, 0.0f, 0.0f)),
  m_pos(glm::vec3(0.0f, 0.0f, 0.0f)),
  m_active(false) {

  initGLFW();

  // Global OpenGL states
  glEnable(GL_DEPTH_TEST);

  initShaders();
  initVAO();
  initTextures();
}

Sandbox::~Sandbox() {
  // Don't delete because glfwTerminate() will? Dangerous using raw pointers
  //delete m_window;

  delete m_shader;
  delete m_groundShader;
}

void Sandbox::run() {

  while (!glfwWindowShouldClose(m_window)) {
    // Time
    float currentFrame = glfwGetTime();
    m_deltaTime = currentFrame -  m_lastFrame;
    m_lastFrame = currentFrame;

    // input
    processInput(m_window);

    // Update cube
    if (m_active) {
      updateCube(m_deltaTime);
      checkCollision();
    }

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind cube texture units
    m_shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2);

    // Projection
    glm::mat4 projection = glm::perspective(glm::radians(m_camera.getZoom()), (float)SCR_WIDTH/ (float)SCR_HEIGHT, 0.1f, 100.0f);
    m_shader->setMat4("projection", projection);

    // View
    glm::mat4 view = m_camera.GetViewMatrix();
    m_shader->setMat4("view", view);

    // Model
    glm::mat4 model = glm::mat4(1.0f);

    if (m_active) {
      // Calculate new position
      model = glm::translate(model, m_pos);
      model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
      m_shader->setMat4("model", model);

      // Draw call for cube
      glBindVertexArray(m_cubeVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }


    // Set texture unit
    m_groundShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_groundTexture);

    // Set transformation matrices, projection and view the same as for cubes
    m_groundShader->setMat4("projection", projection);
    m_groundShader->setMat4("view", view);

    model = glm::mat4(1.0f);
    m_groundShader->setMat4("model", model);

    // Draw call for ground
    glBindVertexArray(m_groundVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    // check and call events and swap the buffers
    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }

  // Deallocate resources when no longer necessary
  glDeleteVertexArrays(1, &m_cubeVAO);
  glDeleteVertexArrays(1, &m_groundVAO);
  glDeleteBuffers(1, &m_cubeVBO);
  glDeleteBuffers(1, &m_groundVBO);
  glDeleteBuffers(1, &m_groundEBO);

  glfwTerminate();
}

void Sandbox::throwNewCube() {
  // Called from mouse input, get direction and set velocity and position
  m_active = true;
  m_pos = m_camera.getPosition();
  m_vel = m_camera.getFront() * 10.0f;
}

void Sandbox::updateCube(float dt) {
  // Calculate new offset
  float dx = m_vel.x * dt;
  float dy = m_vel.y * dt - GRAVITY*dt*dt/2.0f;
  float dz = m_vel.z * dt;

  // Calculate new velocity
  m_vel = glm::vec3(m_vel.x, m_vel.y - GRAVITY * dt, m_vel.z);

  // Add offset to position
  m_pos = m_pos + glm::vec3(dx, dy, dz);
}

void Sandbox::checkCollision() {
  // Check if y-value is < 0, set active to false to stop rendering
  if (m_pos.y < 0) {
    m_active = false;
  }
}

void Sandbox::initGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  glfwSetWindowUserPointer(m_window, this);
  if (m_window == NULL) {
    std::cout << "Failed to create a GLFW window" << std::endl;
    glfwTerminate();
    m_statusOK = false;
  }
  glfwMakeContextCurrent(m_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    m_statusOK = false;
  }

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
  glfwSetCursorPosCallback(m_window, mouse_callback);
  glfwSetMouseButtonCallback(m_window, mouse_button_callback);

  // Mouse input
  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Sandbox::initShaders() {
  // Create shader program
  m_shader = new Shader("../src/shader.vs", "../src/shader.fs");
  m_groundShader = new Shader("../src/shader.vs", "../src/groundShader.fs");
}

void Sandbox::initVAO() {

  // Cube data
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  glGenVertexArrays(1, &m_cubeVAO);
  glGenBuffers(1, &m_cubeVBO);

  // VAO
  glBindVertexArray(m_cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Allowed to undbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  float ground[] = {
    -10.0f,  0.0f,   10.0f,   0.0f,   0.0f, // Bottom left
    -10.0f,  0.0f,  -10.0f,   0.0f,   1.0f, // Top left
     10.0f,  0.0f,  -10.0f,   1.0f,   1.0f, // Top rigth
     10.0f,  0.0f,   10.0f,   1.0f,   0.0f  // Bottom right
  };

  unsigned int groundIndices[] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
  };

  glGenVertexArrays(1, &m_groundVAO);
  glGenBuffers(1, &m_groundVBO);
  glGenBuffers(1, &m_groundEBO);

  // VAO and VBO
  glBindVertexArray(m_groundVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_groundVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

  // EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_groundEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Texture
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Allowed to undbind, dont unbind EBO before VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

}
void Sandbox::initTextures() {
  glGenTextures(1, &m_texture1);
  glGenTextures(1, &m_texture2);
  glGenTextures(1, &m_groundTexture);
  glBindTexture(GL_TEXTURE_2D, m_texture1);

  // Set the texture wrapping/filtering options (currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Load and generate first texture
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load("../src/resources/container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else  {
    std::cout << "Failed to load texture" << std::endl;
  }

  glBindTexture(GL_TEXTURE_2D, m_texture2);

  // Set the texture wrapping/filtering options (currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Load and generate second texture
  data = stbi_load("../src/resources/awesomeFace.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else  {
    std::cout << "Failed to load texture" << std::endl;
  }

  // Set texture units to uniforms
  m_shader->use();
  m_shader->setInt("texture1", 0);
  m_shader->setInt("texture2", 1);

  glBindTexture(GL_TEXTURE_2D, m_groundTexture);

  // Set the texture wrapping/filtering options (currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Load and generate ground texture
  data = stbi_load("../src/resources/ground.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  m_groundShader->use();
  m_groundShader->setInt("texture1", 0);
}

void Sandbox::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void Sandbox::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  Sandbox* sandbox = static_cast<Sandbox*>(glfwGetWindowUserPointer(window));
  sandbox->mouse_callback(xpos, ypos);
}

void Sandbox::mouse_callback(double xpos, double ypos) {
  // If entering the window for the first time
  if (m_firstMouse) {
    m_lastX = xpos;
    m_lastY = ypos;
    m_firstMouse = false;
  }

  float xoffset = xpos - m_lastX;
  float yoffset = m_lastY - ypos;
  m_lastX = xpos;
  m_lastY = ypos;

  m_camera.processMouseMovement(xoffset, yoffset);
}

void Sandbox::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  Sandbox* sandbox = static_cast<Sandbox*>(glfwGetWindowUserPointer(window));
  sandbox->mouse_button_callback(button, action, mods);
}

void Sandbox::mouse_button_callback(int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (!m_active) {
      throwNewCube();
    }
  }
}

void Sandbox::processInput(GLFWwindow* window) {
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
    m_camera.processKeyboard(Camera_Movement::FORWARD, m_deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    m_camera.processKeyboard(Camera_Movement::BACKWARD, m_deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    m_camera.processKeyboard(Camera_Movement::LEFT, m_deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    m_camera.processKeyboard(Camera_Movement::RIGHT, m_deltaTime);
  }
}
