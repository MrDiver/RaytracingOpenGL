#include "spdlog/common.h"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <gl.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <window.hpp>

#include "ezgl.hpp"

using namespace glm;

void error_callback(int32_t error, const char *description)
{
    spdlog::error(description);
}

static void key_callback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 uv;

    Vertex(glm::vec3 pos, glm::vec2 uv)
    {
        this->pos = pos;
        this->uv = uv;
    }
};

int main()
{
    // spdlog::set_level(spdlog::level::debug);
    // Initialized GLFW
    glfwSetErrorCallback(error_callback);

    // Create a Window
    Window window(800, 400, "Ray Tracer");

    // Set callback
    window.setKeyCallback(key_callback);

    std::vector<Vertex> vertices = {
        Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),  // top right
        Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),   // bottom right
        Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)), // top left
        Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),   // bottom right
        Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)), // top left
        Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),  // top right
    };

    ez::VertexBuffer quad_vbo;
    quad_vbo.setData(vertices.data(), vertices.size());

    ez::VertexArray vao;
    quad_vbo.bind();
    vao.attributes({
        {GL_FLOAT, 3},
        {GL_FLOAT, 2}
    });

    ez::Program quad_program("shaders/quad.vsh", "shaders/quad.fsh");

    // ImGui Variables
    float viewport_size = 2.0;
    float focal_length = 1.0;
    float camera_z = 1.0;

    while (!window.shouldClose())
    {
        double time = glfwGetTime();
        // window.setClearColor(sin(time), -sin(time), 0, 1);

        // START RENDERING
        window.startDrawing();

        quad_program.use();
        quad_program.setFloat("window_width", window.width);
        quad_program.setFloat("window_height", window.height);
        quad_program.setFloat("viewport_height", viewport_size);
        quad_program.setFloat("focal_length", focal_length);
        quad_program.setFloat("camera_z", camera_z);

        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui::Begin("<3");
        ImGui::SliderFloat("Viewport Size", &viewport_size, 1.0, 10.0);
        ImGui::SliderFloat("Focal Length", &focal_length, 1.0, 50.0);
        ImGui::SliderFloat("Camera Z", &camera_z, 0.0, 50.0);
        ImGui::End();

        window.endDrawing();
        // END RENDERING
    }

    return EXIT_SUCCESS;
}
