#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // Get required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
  // Program ID
  unsigned int ID;

  // Constructor
  Shader(const char* vertexPath, const char* fragmentPath);

  // Activate/use program
  void use();

  // Utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};

#endif