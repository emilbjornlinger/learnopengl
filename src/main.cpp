#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderOrangeSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";
const char *fragmentShaderYellowSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"  FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

  // Compile vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Check compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Compile fragmentOrange shader
  unsigned int fragmentShaderOrange;
  fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderOrange, 1, &fragmentShaderOrangeSource, NULL);
  glCompileShader(fragmentShaderOrange);

  // Check compile errors
  glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT_ORANGE::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Compile fragmentYellow shader
  unsigned int fragmentShaderYellow;
  fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderYellow, 1, &fragmentShaderYellowSource, NULL);
  glCompileShader(fragmentShaderYellow);

  // Check compile errors
  glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT_YELLOW::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Create shader program1
  unsigned int shaderProgram1;
  shaderProgram1 = glCreateProgram();
  glAttachShader(shaderProgram1, vertexShader);
  glAttachShader(shaderProgram1, fragmentShaderOrange);
  glLinkProgram(shaderProgram1);

  // Check link errors
  glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_PROGRAM_1::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // Create shader program2
  unsigned int shaderProgram2;
  shaderProgram2 = glCreateProgram();
  glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShaderYellow);
  glLinkProgram(shaderProgram2);

  // Check link errors
  glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_PROGRAM_2::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // Delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderOrange);
  glDeleteShader(fragmentShaderYellow);


  // Data
  // float vertices[] = {
  //   0.5f, 0.5f, 0.0f,
  //   0.5f, -0.5f, 0.0f,
  //   -0.5f, -0.5f, 0.0f,
  //   -0.5f, 0.5f, 0.0f
  // };
  float vertices1[] = {
    -0.8, -0.5, 0.0,
    -0.5, 0.5, 0.0,
    -0.2, -0.5, 0.0
  };
  float vertices2[] = {
    0.2, -0.5, 0.0,
    0.5, 0.5, 0.0,
    0.8, -0.5, 0.0
  };
  unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3 // second tringle
  };

  // VAO setup
  unsigned int VBOs[2], VAOs[2];
  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);
  // VAO1
  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

  // Vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // VAO2
  glBindVertexArray(VAOs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

  // Vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Allowed to undbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);


  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram1);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shaderProgram2);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);


    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Deallocate resources when no longer necessary
  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);

  glfwTerminate();

  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
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
}
