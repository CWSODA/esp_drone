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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow* window);

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

glm::vec3 normalized_vec3(float x, float y, float z) {
    return glm::normalize(glm::vec3(x, y, z));
}

void set_lights(Shader& shader);