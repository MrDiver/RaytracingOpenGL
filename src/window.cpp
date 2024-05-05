#include "window.hpp"
#include "GLFW/glfw3.h"
#include <spdlog/spdlog.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

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

    // Creation of ImGui
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // IF using Docking Branch
    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/TTF/LilexNerdFont-Regular.ttf", 20);

    ImGui_ImplGlfw_InitForOpenGL(this->w, true);
    ImGui_ImplOpenGL3_Init();
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::endDrawing()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwPollEvents();
    glfwSwapBuffers(this->w);
}

void Window::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}
