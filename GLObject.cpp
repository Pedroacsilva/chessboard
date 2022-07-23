#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include <vector>
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

#include "GLObject.hpp"

GLObject::GLObject(VertexArray va) : m_VA(va) {}

GLObject::GLObject() {}

GLObject::~GLObject() {}

void GLObject::Bind() const { m_VA.Bind(); }

void GLObject::Unbind() const { m_VA.Unbind(); }

Cube::Cube() {
  float face_positions[] = {
      // Vertex Coordinates  Tex Coords       RGB  Values
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, // 0
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 1
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,   // 2
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 3
      //-----------------------------------------------------------------------------
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, // 4
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 5
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,   // 6
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 7
      //-----------------------------------------------------------------------------
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 8
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, // 9
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 10
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,   // 11
      //-----------------------------------------------------------------------------
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 12
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, // 13
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 14
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,   // 15
      //-----------------------------------------------------------------------------
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 16
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,   // 17
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 18
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, // 19
      //-----------------------------------------------------------------------------
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, // 20
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,  // 21
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, // 22
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f // 23
  };

  unsigned int indices[] = {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,
                            8,  9,  10, 10, 11, 8,  12, 13, 14, 14, 15, 12,
                            16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

  m_VB.Push(GL_ARRAY_BUFFER, 6 * 4 * 9 * sizeof(float), face_positions,
            GL_STATIC_DRAW);
  m_IB.Push(indices, 6 * 6);
  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_Layout.Push<float>(4, "Vertex Colors");
  m_VA.AddBuffer(m_VB, m_Layout);
}

Cube::~Cube() {}

Pyramid::Pyramid() {
  float vtx_positions[] = {0.0f,  0.5f, 0.0f,         0.0f,  1.0f,      0.5f, 0.5f, 0.5f, 1.0f,
                           -0.5f, 0.0f, -0.5f,        0.0f,  0.0f,      0.5f, 0.5f, 0.5f, 1.0f,
                           0.5f,  0.0f, -0.5f,        0.25f, 0.0f,      0.5f, 0.5f, 0.5f, 1.0f,
                           0.5f,  0.0f, 0.5f,         0.5f,  0.0f,      0.5f, 0.5f, 0.5f, 1.0f,
                           -0.5f, 0.0f, 0.5f,         0.75f, 0.0f,      0.5f, 0.5f, 0.5f, 1.0f};
  unsigned int indices[] = {0, 1, 2, 0, 2, 3, 0, 3, 4,
                            0, 4, 1, 4, 1, 2, 2, 3, 4};

  m_VB.Push(GL_ARRAY_BUFFER, sizeof(vtx_positions), vtx_positions,
            GL_STATIC_DRAW);
  m_IB.Push(indices, 6 * 3);
  m_Layout.Push<float>(3, "Vertex Coordinates");
  m_Layout.Push<float>(2, "Texture Coordinates");
  m_Layout.Push<float>(4, "Vertex Colors");
  m_VA.AddBuffer(m_VB, m_Layout);
};

Pyramid::~Pyramid() {}