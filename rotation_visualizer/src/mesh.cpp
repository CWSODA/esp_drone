#include <glad/glad.h>

#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setup_mesh();
}

void Mesh::setup_mesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, normal));
    // vertex texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader) {
    // index from 0
    unsigned int n_diffuse = 0;
    unsigned int n_specular = 0;

    shader.use();
    // uniform naming scheme:
    // material.texture_<type><index>
    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string uniform_name = "material.texture_";
        if (textures[i].get_type() == TextureType::DIFFUSE) {
            uniform_name += "diffuse";
            uniform_name += std::to_string(n_diffuse++);
        } else {
            uniform_name += "specular";
            uniform_name += std::to_string(n_specular++);
        }

        shader.set_int(uniform_name, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].get_id());
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()),
                   GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}