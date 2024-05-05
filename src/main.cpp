#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fstream>
#include <gl.h>
#include <spdlog/spdlog.h>

void error_callback(int32_t error, const char *description)
{
    spdlog::error(description);
}

static void key_callback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
    // Initialized GLFW
    glfwSetErrorCallback(error_callback);

    if (glfwInit() != GLFW_TRUE)
    {
        spdlog::error("GLFW could not be initialized");
        exit(EXIT_FAILURE);
    }

    // Create a Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Ray Tracing", NULL, NULL);
    if (!window)
    {
        spdlog::error("GLFW the window could not be created");
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // Loading glad
    gladLoadGL(glfwGetProcAddress);

    // Window Loop
    int32_t width, height;
    glfwSwapInterval(1);

    // Set callback
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window))
    {
        double time = glfwGetTime();

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.5f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
