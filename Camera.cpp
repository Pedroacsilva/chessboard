#include "Camera.hpp"

#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

#ifndef GL_SETUP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GL_SETUP
#endif

Camera::~Camera() {}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
  /*return glm::lookAt(m_Position, glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));*/
}

/*void Camera::KeyCallback(GLFWwindow *window, int key, int scancode,
                             int action, int mods) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    m_Position += m_Front * 0.001;
  }*/

void Camera::UpdateCameraVectors() {}

/*void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
cameraSpeed; if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos +=
glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}*/