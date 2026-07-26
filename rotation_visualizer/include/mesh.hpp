#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <array>

#include "shader.hpp"
#include "texture.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;

    Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex)
        : position(pos), normal(norm), tex_coord(tex) {}

    Vertex(const float input[8]) {
        position = glm::vec3(input[0], input[1], input[2]);
        normal = glm::vec3(input[3], input[4], input[5]);
        tex_coord = glm::vec2(input[6], input[7]);
    }
};

class Mesh {
   public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<Texture> textures);

    void draw(Shader& shader);

   private:
    unsigned int VAO, VBO, EBO;
    void setup_mesh();
};
