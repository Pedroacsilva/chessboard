// g++ -o chessboard chessboard.cpp imgui.o imgui_impl_glfw.o
// imgui_impl_opengl3.o imgui_draw.o imgui_tables.o imgui_widgets.o
// -I./vendor/stb_image -I./vendor -I./vendor/imgui  -lglfw -lGLEW -lGL -Wall
// -lrt -lm -ldl

/*
TODO:
  - Se possível, evitar que Camera seja um objecto global.
https://stackoverflow.com/questions/21799746/how-to-glfwsetkeycallback-for-different-classes
  Perceber glfwSet/GetWindowUserPointer melhor -- julgo que seria possível desta
forma mas teria de modificar a class Camara para ter um ponteiro para a janela
em que trabalha?
  - Implementar callbacks para rato -- mudar o alvo da funçao LookAt da class
Camara
  - Criar classes filhas de GLObjects cujos construtores construam um VAO, VBO e
IBO para desenhar:
      - Cubo
      - Esfera
      - Cilindro
      - Cone
      - Pirầmide com N vértices na base
      - etc...
  - Usar vectors para armzenar dados de vertices
https://discord.com/channels/103678524375699456/103678524375699456/999338270846627880
*/

#include "Camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL

#ifndef GL_SETUP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GL_SETUP
#endif

#ifndef STB_I
#include "vendor/stb_image/stb_image.cpp"
#include "vendor/stb_image/stb_image_print.cpp"
#define STB_I
#endif

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <string>

#ifndef VA
#include "VertexArray.cpp"
#define VA
#endif

#ifndef VB
#include "VertexBuffer.cpp"
#define VB
#endif

#ifndef VBL
#include "VertexBufferLayout.hpp"
#define VBL
#endif

#ifndef IB
#include "IndexBuffer.cpp"
#define IB
#endif

#ifndef SHADERS
#include "Shaders.cpp"
#define SHADERS
#endif

#ifndef TEXTURES
#include "Texture.cpp"
#define TEXTURES
#endif

#include "Camera.cpp"
#include "GLObject.cpp"
#include "Renderer.cpp"

#define GLEW_STATIC

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const GLchar *message, const void *userParam) {
  std::cout /* << "(" <<__FILE__ << ":" << __LINE__ << ")" */
      << "GL CALLBACK: type = " << std::hex << "0x" << type << ", severity = 0x"
      << severity << ", message = " << std::dec << message
      << ". Source:  " << source << "\n";
}

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f,
              0.0f);

GLFWwindow *window;

bool wire = false;
void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_TRUE)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_TRUE)
    camera.m_Position += camera.m_Front * 0.1f;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_TRUE)
    camera.m_Position -= camera.m_Front * 0.1f;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_TRUE)
    camera.m_Position += camera.m_Right * 0.1f;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_TRUE)
    camera.m_Position -= camera.m_Right * 0.1f;
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_TRUE) {
    wire = !wire;
    (wire) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
           : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

float lastX = 400, lastY = 300, yaw = -90.0f, pitch = 0.0f;
;
bool firstMouse = true;

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  camera.m_Front = glm::normalize(direction);
  camera.m_Right =
      glm::normalize(glm::cross(camera.m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
}

int main(int argc, char const *argv[]) {

  if (!glfwInit()) {
    std::cout << "Error initializing GLFW.\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Chessboard", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);

  if (glewInit() != GLEW_OK) {
    std::cout << "Error initializing GLEW.\n";
    return -1;
  }

  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  // glEnable(GL_CULL_FACE);
  glDebugMessageCallback(MessageCallback, 0);
  glEnable(GL_BLEND | GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  Shaders colorShader("res/shaders/color.shader");
  // Texture texture("res/textures/die.png");
  //  texture.Bind();
  // colorShader.SetUniform1i("u_Texture", 0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 proj = glm::perspective(glm::radians(80.0f), 1.0f, 0.5f, 5.0f);
  glm::mat4 mvp = proj * view * model;

  colorShader.SetUniformMat4f("u_MVP", mvp);

  Renderer renderer;
  renderer.Clear();
  float RGBA[4] = {0.5f, 0.5f, 0.5f, 1.0f};

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfwGL3_Init(window, true);
  ImGui::StyleColorsDark();
  glfwSetKeyCallback(window, KeyCallback);

  glm::vec3 cube_positions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(1.5f, 0.0f, 0.0f)/*,
      glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f),
      glm::vec3(0.7f, 0.7, 0.0f),   glm::vec3(-0.9f, -0.2f, 0.0f)*/};

  Cube cubo;
  Pyramid piramide;
  while (!glfwWindowShouldClose(window)) {

    renderer.Clear();
    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::SliderFloat4("RGBA", RGBA, 0.0f, 1.0f);

    colorShader.Bind();
    colorShader.SetUniform4f("u_RGBA", RGBA[0], RGBA[1], RGBA[2], RGBA[3]);
    view = camera.GetViewMatrix();
    for (auto elemt : cube_positions) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, elemt);
      mvp = proj * view * model;
      colorShader.SetUniformMat4f("u_MVP", mvp);
      renderer.Draw(cubo.m_VA, cubo.m_IB, colorShader);
      model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
      mvp = proj * view * model;
      colorShader.SetUniformMat4f("u_MVP", mvp);
      renderer.Draw(piramide.m_VA, piramide.m_IB, colorShader);
      piramide.Unbind();
      cubo.Unbind();
    }

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}