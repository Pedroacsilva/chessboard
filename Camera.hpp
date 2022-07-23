#pragma once
#include "glm/glm.hpp"
#include <glm/fwd.hpp>
#ifndef GL_SETUP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GL_SETUP
#endif

class Camera {
public:
  Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
      : m_Position(position), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        m_Up(glm::vec3(0.0f, 1.0f, 0.0f)), m_Right(glm::normalize(glm::cross(m_Front, m_Up))), m_Yaw(yaw), m_Pitch(pitch){
        };
  ~Camera();

  glm::vec3 m_Position;
  glm::vec3 m_Front;
  glm::vec3 m_Up;
  glm::vec3 m_Right;
  glm::vec3 m_WorldUp;
  float m_Yaw, m_Pitch;
  glm::mat4 GetViewMatrix();
private:
  void UpdateCameraVectors();
};