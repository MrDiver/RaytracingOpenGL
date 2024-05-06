#pragma once
#include "spdlog/spdlog.h"
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <utility>
#define GLAD_GL_IMPLEMENTATION
#include <gl.h>

namespace ez
{

GLint getGLTypeSize(GLenum type);

class VertexBuffer
{
  private:
    GLuint id;
    bool dynamic;

  public:
    VertexBuffer(bool dynamic = false);
    ~VertexBuffer();

    void bind();
    template <typename T> void setData(T *data, size_t count);
};

template <typename T> void VertexBuffer::setData(T *data, size_t count)
{
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * count, data, this->dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

class VertexArray
{
  private:
    GLuint id;

  public:
    VertexArray();
    ~VertexArray();

    void bind();
    void attributes(std::initializer_list<std::pair<GLenum, GLint>> elements);
};

class Program
{
  private:
    GLint id;

  public:
    Program(std::string vertex_path, std::string fragment_path);
    ~Program();

    void use();
};

} // namespace ez
