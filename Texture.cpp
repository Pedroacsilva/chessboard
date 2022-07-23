#include "Texture.hpp"
#include <iostream>

#ifndef STB_I
#include "stb_image.cpp"
#include "stb_image_print.cpp"
#define STB_I
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Texture::Texture(const std::string &filepath)
    : m_RendererID(0), m_Filepath(filepath), m_LocalBuffer(nullptr), m_Width(0),
      m_Height(0), m_BPP(0) {
  stbi_set_flip_vertically_on_load(1);
  m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
  if (!m_LocalBuffer)
    std::cout << "Error loading texture image.\n";
  glGenTextures(1, &m_RendererID);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, m_LocalBuffer);
  Unbind();

  if (m_LocalBuffer)
    stbi_image_free(m_LocalBuffer);
}

Texture::~Texture() {}

void Texture::Bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
