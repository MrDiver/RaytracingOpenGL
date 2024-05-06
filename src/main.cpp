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
    // Initialized GLFW
    glfwSetErrorCallback(error_callback);

    // Create a Window
    Window window(1280, 720, "Ray Tracer");

    // Set callback
    window.setKeyCallback(key_callback);


    ez::Program quad_program("shaders/quad.vsh", "shaders/quad.fsh");

    // GLuint tex;
    // size_t width = 100, height = 100;
    // uint8_t data[width * height * 3];
    // for (size_t y = 0; y < height; y++)
    // {
    //     for (size_t x = 0; x < width; x++)
    //     {
    //         data[y * width + x + 0] = 0;
    //         data[y * width + x + 1] = 0;
    //         data[y * width + x + 2] = 0;
    //     }
    // }
    // glGenTextures(1, &tex);
    // glBindTexture(GL_TEXTURE_2D, tex);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
    // GL_UNSIGNED_BYTE, data); glGenerateMipmap(GL_TEXTURE_2D);

    while (!window.shouldClose())
    {
        double time = glfwGetTime();
        // window.setClearColor(sin(time), -sin(time), 0, 1);

        // START RENDERING
        window.startDrawing();
        std::vector<float> vertices = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f,  0.5f, 0.0f,  // top left 
        };

        ez::VertexBuffer quad_vbo;
        quad_vbo.setData(vertices.data(), vertices.size());

        ez::VertexArray vao;
        quad_vbo.bind();
        vao.attributes({
            {GL_FLOAT, 3},
        });

        quad_program.use();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.endDrawing();
        // END RENDERING
    }

    return EXIT_SUCCESS;
}
