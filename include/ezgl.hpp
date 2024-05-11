#pragma once
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <string>
#include <utility>
#define GLAD_GL_IMPLEMENTATION
#include <efsw/efsw.hpp>
#include <gl.h>
#include <glm/glm.hpp>

namespace ez
{

GLint getGLTypeSize(GLenum type);
void checkError();

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

class Program : public efsw::FileWatchListener
{
  private:
    GLint id;
    efsw::FileWatcher watcher;
    bool autoreload;
    std::string vertexPath;
    std::string fragmentPath;
    std::vector<std::string> includedFiles;
    bool needsRecompile = false;
    void compile();

  public:
    Program(std::string const &vertex_path, std::string const &fragment_path, bool autoreload = false);
    ~Program();

    void recompile();
    void use();
    void setInt(std::string const &name, uint32_t value);
    void setFloat(std::string const &name, float value);
    void setVec2(std::string const &name, glm::vec2 const &value);
    void setVec2(std::string const &name, float v1, float v2);
    void setVec3(std::string const &name, glm::vec3 const &value);
    void setVec3(std::string const &name, float v1, float v2, float v3);
    void setVec4(std::string const &name, glm::vec4 const &value);
    void setVec4(std::string const &name, float v1, float v2, float v3, float v4);

    // FileWatchListener
    void handleFileAction(efsw::WatchID watchid, const std::string &dir, const std::string &filename,
                          efsw::Action action, std::string oldFilename) override;
};

class SSBO
{
  private:
    GLuint id;

  public:
    SSBO();
    ~SSBO();
    void bind();
    void layout(GLint binding);
    template <typename T> void setData(T *data, size_t count);
    template <typename T> void setSubData(T *data, size_t start, size_t count);
};
template <typename T> void SSBO::setData(T *data, size_t count)
{
    this->bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * count, data, GL_DYNAMIC_COPY);
}

template <typename T> void SSBO::setSubData(T *data, size_t start, size_t count)
{
    this->bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * start, sizeof(T) * count, data + start);
}

} // namespace ez
