cmake_minimum_required(VERSION 3.20)

# CREATE EXECUTABLE

project("ray")

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
