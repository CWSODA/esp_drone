cmake_minimum_required(VERSION 3.10)

project(MyOpenGLApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include ImGui as subdir
add_subdirectory(imgui)

add_library(glad src/glad.c)

# Find GLFW
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

# app is the executable
add_executable(app src/main.cpp src/texture.cpp src/model.cpp src/mesh.cpp src/serial_port.cpp)

include_directories(include)

# 
target_link_libraries(app glad glfw assimp::assimp imgui)
target_include_directories(imgui PUBLIC /opt/homebrew/include)