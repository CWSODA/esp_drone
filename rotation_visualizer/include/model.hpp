#pragma once

#include <vector>
#include <string>
#include <assimp/scene.h>

#include "shader.hpp"
#include "mesh.hpp"

class Model {
   public:
    Model(const char* path) { load_model(path); }
    void draw(Shader& shader);

   private:
    std::vector<Mesh> meshes;
    std::string directory;
    void load_model(std::string path);
    void process_nodes(const aiNode* node, const aiScene* scene);
    Mesh process_mesh(const aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_texture(aiMaterial* material,
                                               aiTextureType type);
};

std::vector<Texture> load_material_texture(aiMaterial* material,
                                           aiTextureType type);