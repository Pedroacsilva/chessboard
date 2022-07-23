#pragma once
#include <string>
#include <unordered_map>
struct ShaderProgramSources {
  std::string VertexSource;
  std::string FragmentSource;
};

class Shaders {
  std::string m_FilePath;
  unsigned int m_RendererID;
  std::unordered_map<std::string, int> m_UniformLocationCache;

public:
  Shaders(const std::string &);
  Shaders(const std::string &, const std::string &);
  ~Shaders();

  void Bind() const;
  void Unbind() const;

  unsigned int GetUniformLocation(const std::string &);
  void SetUniform1i(const std::string &name, int value);
  void SetUniform1f(const std::string &name, float value);
  void SetUniform4f(const std::string &name, float f0, float f1, float f2,
                    float f3);
  void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix);

private:
  unsigned int CompileShader(unsigned int type, const std::string &source);
  unsigned int CreateShader(const std::string &vertexShader,
                            const std::string &fragShader);
  ShaderProgramSources ParseShader(const std::string &);
};