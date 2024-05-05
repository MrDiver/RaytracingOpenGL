#pragma once
#include <cstdint>

#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <gl.h>

class Window
{
  private:
    GLFWwindow *w;

  public:
    int32_t width, height;
    Window(int32_t width, int32_t height, const char *name);
    ~Window();

    void setKeyCallback(GLFWkeyfun callback);
    void close();
    bool shouldClose();
    void startDrawing();
    void endDrawing();
    void setClearColor(float r, float g, float b, float a);
};
