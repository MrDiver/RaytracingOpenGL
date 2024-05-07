#pragma once
#include "spdlog/spdlog.h"
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <string_view>
#include <utility>
#define GLAD_GL_IMPLEMENTATION
#include <gl.h>
#include <glm/glm.hpp>

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
    Program(std::string const &vertex_path, std::string const &fragment_path);
    ~Program();

    void use();
    void setFloat(std::string const &name, float value);
    void setVec2(std::string const &name, glm::vec2 const &value);
    void setVec2(std::string const &name, float v1, float v2);
    void setVec3(std::string const &name, glm::vec3 const &value);
    void setVec3(std::string const &name, float v1, float v2, float v3);
    void setVec4(std::string const &name, glm::vec4 const &value);
    void setVec4(std::string const &name, float v1, float v2, float v3, float v4);
};

} // namespace ez
