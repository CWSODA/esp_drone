#pragma once

#include "mesh.hpp"
#include <glm/gtc/matrix_transform.hpp>

Mesh get_plane_mesh() {
    glm::vec3 normal(0.0f, 1.0f, 0.0f);
    glm::vec2 tex_coords(0.0f, 0.0f);
    std::vector<Vertex> vertices = {
        Vertex(glm::vec3(-0.5f, 0.0f, 0.5f), normal, tex_coords),
        Vertex(glm::vec3(0.5f, 0.0f, 0.5f), normal, tex_coords),
        Vertex(glm::vec3(-0.5f, 0.0f, -0.5f), normal, tex_coords),
        Vertex(glm::vec3(0.5f, 0.0f, -0.5f), normal, tex_coords),
    };
    std::vector<unsigned int> indices = {0, 1, 2, 1, 2, 3};
    std::vector<Texture> textures;
    return Mesh(vertices, indices, textures);
}
