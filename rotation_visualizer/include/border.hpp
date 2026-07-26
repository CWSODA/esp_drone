#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "mesh.hpp"

void draw_border(Mesh &mesh, glm::vec3 &color, float multiplier,
                 glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) {
    static Shader border_shader("../src/shaders/vs/pos_only.vert",
                                "../src/shaders/fs/single_color.frag");

    glm::mat4 scaled_model = glm::scale(model, glm::vec3(multiplier));

    border_shader.use();
    border_shader.set_mat4("model", scaled_model);
    border_shader.set_mat4("view", view);
    border_shader.set_mat4("projection", projection);
    border_shader.set_vec3("color", color);

    mesh.draw(border_shader);
}