#include "spdlog/common.h"
#include <vector>
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <gl.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <window.hpp>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"
#include "imgui_internal.h"

#include "ezgl.hpp"

using namespace glm;

void error_callback(int32_t error, const char *description)
{
    spdlog::error(description);
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

struct Sphere
{
    alignas(16) glm::vec3 origin;
    alignas(16) glm::vec3 color;
    float radius;

    Sphere(glm::vec3 origin, float radius, glm::vec3 color = glm::vec3(0.7, 0.7, 0.7))
    {
        this->origin = origin;
        this->radius = radius;
        this->color = color;
    }
};

struct GlobalData
{
    float viewport_size = 2.0;
    float focal_length = 7.0;
    float camera_z = 17.0;
    float t_min = 0.1;
    float t_max = 100.0;
    std::vector<Sphere> spheres;
};

static void key_callback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    GlobalData *data = (GlobalData *)glfwGetWindowUserPointer(window);
    data->camera_z += yoffset;
}

int main()
{
    // spdlog::set_level(spdlog::level::debug);
    // Initialized GLFW
    glfwSetErrorCallback(error_callback);

    // Create a Window
    Window window(1280, 720, "Ray Tracer");

    // Set callback
    window.setKeyCallback(key_callback);
    window.setScrollCallback(scroll_callback);

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
    GlobalData globaldata;
    window.setUserPointer(&globaldata);
    double lastTime = glfwGetTime();
    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(glm::vec3(0, 0, 0), 0.5, glm::vec3(1, 0, 0)));
    spheres.push_back(Sphere(glm::vec3(-2, 0, 0), 0.3, glm::vec3(0, 1, 0)));
    spheres.push_back(Sphere(glm::vec3(2, 0, 0), 0.3, glm::vec3(0, 0, 1)));
    ez::SSBO sphereSSBO;
    sphereSSBO.setData(spheres.data(), spheres.size());

    while (!window.shouldClose())
    {
        double time = glfwGetTime();
        // window.setClearColor(sin(time), -sin(time), 0, 1);

        // START RENDERING
        window.startDrawing();

        quad_program.use();
        quad_program.setFloat("window_width", window.width);
        quad_program.setFloat("window_height", window.height);
        quad_program.setFloat("viewport_height", globaldata.viewport_size);
        quad_program.setFloat("focal_length", globaldata.focal_length);
        quad_program.setFloat("camera_z", globaldata.camera_z);
        quad_program.setFloat("t_min", globaldata.t_min);
        quad_program.setFloat("t_max", globaldata.t_max);
        quad_program.setInt("numSpheres", spheres.size());

        sphereSSBO.bind();
        sphereSSBO.layout(3);
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui::Begin("<3");

        ImGui::Text("%f", 1 / (glfwGetTime() - lastTime));
        lastTime = glfwGetTime();
        ImGui::SliderFloat("Viewport Size", &globaldata.viewport_size, 1.0, 10.0);
        ImGui::SliderFloat("Focal Length", &globaldata.focal_length, 1.0, 50.0);
        ImGui::SliderFloat("Camera Z", &globaldata.camera_z, 0.0, 50.0);
        ImGui::SliderFloat("Min Clip", &globaldata.t_min, 0.0, 10.0);
        ImGui::SliderFloat("Max Clip", &globaldata.t_max, 10.0, 100.0);
        if (ImGui::Button("Add Sphere", ImVec2(30, 30)))
        {
            spheres.push_back(Sphere(glm::vec3(0, 0, 0), 1.0));
            sphereSSBO.setData(spheres.data(), spheres.size());
        }

        for (uint32_t i = 0; i < spheres.size(); i++)
        {
            ImGui::PushID(i);
            if (ImGui::Button("Delete"))
            {
                spheres.erase(spheres.begin() + i);
                sphereSSBO.setData(spheres.data(), spheres.size());
            }
            ImGui::SameLine();
            if (ImGui::CollapsingHeader("Sphere"))
            {
                bool positionUpdated = ImGui::SliderFloat3("Position", &spheres[i].origin.x, -2, 2);
                bool radiusUpdated = ImGui::SliderFloat("Radius", &spheres[i].radius, -2, 2);
                bool colorUpdated = ImGui::ColorPicker3("Color", &spheres[i].color.x);
                if (positionUpdated || colorUpdated || radiusUpdated)
                {
                    sphereSSBO.setSubData(spheres.data(), i, 1);
                }
            }

            ImGui::PopID();
        }
        ImGui::End();

        window.endDrawing();
        // END RENDERING
    }

    return EXIT_SUCCESS;
}
