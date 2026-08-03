#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "serial_port.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

int fps(float delta_time) { return round(1 / delta_time); }

constexpr glm::mat4 get_transform_matrix(glm::vec3 position) {
    return glm::translate(glm::mat4(1.0f), position);
}

constexpr glm::mat4 get_transform_matrix(float x, float y, float z) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

constexpr float shifted_sin(float value) { return 1.0f + 0.5f * sin(value); }

glm::vec3 normalized_vec3(float x, float y, float z) { return glm::normalize(glm::vec3(x, y, z)); }

void set_lights(Shader& shader);

// convert from normal space to OpenGL space
glm::vec3 gl_space(float x, float y, float z) { return glm::vec3(y, z, -x); }
glm::vec3 gl_space(glm::vec3 vec) { return glm::vec3(vec.y, vec.z, -vec.x); }

void disp(glm::vec3 vec, const char* label = nullptr) {
    if (label == nullptr) {
        printf("%f, %f, %f\n", vec.x, vec.y, vec.z);
    } else {
        printf("%s: %f, %f, %f\n", label, vec.x, vec.y, vec.z);
    }
}
void disp(glm::quat q, const char* label = nullptr) {
    if (label == nullptr) {
        printf("%f, %f, %f, %f\n", q.w, q.x, q.y, q.z);
    } else {
        printf("%s: %f, %f, %f, %f\n", label, q.w, q.x, q.y, q.z);
    }
}