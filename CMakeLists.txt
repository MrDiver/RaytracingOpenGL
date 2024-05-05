cmake_minimum_required(VERSION 3.20)

# CREATE EXECUTABLE

project("ray")

# CONFIG FOR CPP 20
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
    "src/main.cpp"
    "src/window.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")


# LINK GLFW
set(GLFW_LIBRARY_TYPE "SHARED")
set(GLFW_STANDALONE OFF)
set(GLFW_BUILD_EXAMPLES ${GLFW_STANDALONE})
set(GLFW_BUILD_TESTS ${GLFW_STANDALONE})
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

add_subdirectory("external/glfw")
target_link_libraries(${PROJECT_NAME} glfw)
target_include_directories(${PROJECT_NAME} PRIVATE glfw)

# ADD glad
add_subdirectory("external/glad")
target_include_directories(${PROJECT_NAME} PRIVATE GLAD)
target_link_libraries(${PROJECT_NAME} GL)
target_link_libraries(${PROJECT_NAME} GLAD)

# LINK SPDLOG
add_subdirectory("external/spdlog")
target_link_libraries(${PROJECT_NAME} spdlog)
target_include_directories(${PROJECT_NAME} PRIVATE spdlog)

# LINK IMGUI
set(IMGUI_BACKEND "glfw")
set(IMGUI_API "opengl3")
file(GLOB IMGUI_SOURCES "external/imgui/*.cpp")
file(GLOB IMGUI_HEADERS "external/imgui/*.h")
message(${IMGUI_SOURCES})
add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS}
    "external/imgui/backends/imgui_impl_${IMGUI_BACKEND}.cpp"
    "external/imgui/backends/imgui_impl_${IMGUI_BACKEND}.h"
    "external/imgui/backends/imgui_impl_${IMGUI_API}.cpp"
    "external/imgui/backends/imgui_impl_${IMGUI_API}.h"
)
target_include_directories(imgui PRIVATE "external/imgui")
target_link_libraries(${PROJECT_NAME} imgui)
#target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "external/imgui")
