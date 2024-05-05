#include "window.hpp"
#include "GLFW/glfw3.h"
#include <spdlog/spdlog.h>

Window::Window(int32_t width, int32_t height, const char *name)
{
    if (glfwInit() != GLFW_TRUE)
    {
        spdlog::error("GLFW could not be initialized");
        exit(EXIT_FAILURE);
    }

    // Create a Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    this->w = glfwCreateWindow(1280, 720, "Ray Tracing", NULL, NULL);
    if (!this->w)
    {
        spdlog::error("GLFW the window could not be created");
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(this->w);

    // Loading glad
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
    glClearColor(0.5f, 0.2f, 0.2f, 0.0f);
}

Window::~Window()
{
    glfwDestroyWindow(this->w);
    glfwTerminate();
}

void Window::setKeyCallback(GLFWkeyfun callback)
{
    glfwSetKeyCallback(this->w, callback);
}

void Window::close()
{
    glfwSetWindowShouldClose(this->w, GLFW_TRUE);
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(this->w);
}

void Window::startDrawing()
{
    glfwGetFramebufferSize(this->w, &this->width, &this->height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::endDrawing()
{
    glfwPollEvents();
    glfwSwapBuffers(this->w);
}

void Window::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}
