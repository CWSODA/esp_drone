#include "model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::draw(Shader& shader) {
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader);
    }
}

void Model::load_model(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    // checks if scene is a nullptr, checks if incomplete bit is set
    // checks if the pointer to the root node is nullptr
    if (!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    process_nodes(scene->mRootNode, scene);
}

void Model::process_nodes(const aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // mMeshes for nodes are indices for the mesh array in the scene
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_nodes(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(const aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // process vertex
        glm::vec3 vertex_position;
        vertex_position.x = mesh->mVertices[i].x;
        vertex_position.y = mesh->mVertices[i].y;
        vertex_position.z = mesh->mVertices[i].z;

        glm::vec3 vertex_normal;
        vertex_normal.x = mesh->mNormals[i].x;
        vertex_normal.y = mesh->mNormals[i].y;
        vertex_normal.z = mesh->mNormals[i].z;

        // only taking the first texture coord
        // first makes sure it isnt a nullptr
        glm::vec2 vertex_tex_coord = glm::vec2(0.0f);
        if (mesh->mTextureCoords[0]) {
            vertex_tex_coord.x = mesh->mTextureCoords[0][i].x;
            vertex_tex_coord.y = mesh->mTextureCoords[0][i].y;
        }
        Vertex vertex(vertex_position, vertex_normal, vertex_tex_coord);

        vertices.push_back(vertex);
    }

    // process indices from faces
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process textures (materials)
    unsigned int material_index = mesh->mMaterialIndex;
    if (material_index >= 0) {
        aiMaterial* material = scene->mMaterials[material_index];
        std::vector<Texture> diffuse_maps =
            load_material_texture(material, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuse_maps.begin(),
                        diffuse_maps.end());
        std::vector<Texture> specular_maps =
            load_material_texture(material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), specular_maps.begin(),
                        specular_maps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_texture(aiMaterial* material,
                                                  aiTextureType type) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString path;
        material->GetTexture(type, i, &path);
        std::string filename = std::string(path.C_Str());
        TextureType tex_type = static_cast<TextureType>(type);

        std::string filepath = directory + '/' + filename;
        Texture texture = TexDict::init_texture(filepath, tex_type);

        textures.push_back(texture);
    }

    return textures;
}