#include "Shaders.hpp"
#include "glm/glm.hpp"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <alloca.h>

#ifndef GL_SETUP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GL_SETUP
#endif

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shaders::Shaders(const std::string &filepath)
    : m_FilePath(filepath), m_RendererID(0) {
  ShaderProgramSources sources = ParseShader(filepath);
  std::cout << "Vertex Shader: \n"
            << sources.VertexSource << "\n"
            << "Fragment Shader: \n"
            << sources.FragmentSource << "\n";
  m_RendererID = CreateShader(sources.VertexSource, sources.FragmentSource);
  glUseProgram(m_RendererID);
}

Shaders::Shaders(const std::string &, const std::string &) {}

Shaders::~Shaders() {}

void Shaders::Bind() const { glUseProgram(m_RendererID); }

void Shaders::Unbind() const { glUseProgram(0); }

unsigned int Shaders::GetUniformLocation(const std::string &name) {
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];
  int location = glGetUniformLocation(m_RendererID, name.c_str());
  if (location == -1)
    std::cout << "[" <<m_FilePath << "]Warning: Uniform " << name << " not found.\n";
  else
    std::cout << "[" <<m_FilePath << "]Uniform " << name << " in location " << location << ".\n";
  m_UniformLocationCache[name] = location;
  return location;
}

void Shaders::SetUniform1i(const std::string &name, int value) {
  glUniform1i(GetUniformLocation(name), value);
}

void Shaders::SetUniform1f(const std::string &name, float value) {
  glUniform1f(GetUniformLocation(name), value);
}

void Shaders::SetUniform4f(const std::string &name, float f0, float f1,
                           float f2, float f3) {
  glUniform4f(GetUniformLocation(name), f0, f1, f2, f3);
}

void Shaders::SetUniformMat4f(const std::string &name,
                              const glm::mat4 &matrix) {
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
      /*unsigned int transformLoc = glGetUniformLocation(m_RendererID, name.c_str());
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));*/

}


unsigned int Shaders::CompileShader(unsigned int type,
                                    const std::string &source) {

  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // Error handling
  int errorResult;
  glGetShaderiv(id, GL_COMPILE_STATUS, &errorResult);

  if (errorResult == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = static_cast<char *>(alloca(length * sizeof(char)));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Error compiling "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << "shader: " << message << "\n";
    glDeleteShader(id);
  }

  return id;
}

unsigned int Shaders::CreateShader(const std::string &vertexShader,
                                   const std::string &fragShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

ShaderProgramSources Shaders::ParseShader(const std::string &filepath) {
  std::ifstream stream(filepath);
  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
  std::stringstream ss[2];
  ShaderType shaderType = ShaderType::NONE;
  std::string line;
  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos)
        // set mode to vertex
        shaderType = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        // set mode to fragment
        shaderType = ShaderType::FRAGMENT;
    } else {
      ss[static_cast<int>(shaderType)] << line << "\n";
    }
  }
  return {ss[0].str(), ss[1].str()}; // Return Vertex & Fragment source code
}