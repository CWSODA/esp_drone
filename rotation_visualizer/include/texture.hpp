#pragma once

#include <iostream>

#include <glad/glad.h>
#include <assimp/scene.h>
#include <string>
#include <unordered_map>

enum class TextureType {
    DIFFUSE = aiTextureType_DIFFUSE,
    SPECULAR = aiTextureType_SPECULAR,
    NONE,
};

enum class WrapType {
    NEAREST = GL_NEAREST,
    MIRROR_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
};

enum class FilterType { NEAREST = GL_NEAREST, BILINEAR = GL_LINEAR };

class Texture {
   public:
    Texture(const std::string& texture_path,
            TextureType type = TextureType::NONE);
    Texture(unsigned int id, TextureType type = TextureType::NONE);
    // ~Texture();

    void bind(unsigned int texture_unit);

    [[nodiscard]] unsigned int get_id() { return _id; }
    [[nodiscard]] TextureType get_type() { return _type; }
    void set_wrap_type(WrapType type);
    void set_filter_type(FilterType type);

   private:
    // both are set once in constructor and cannot be changed after
    unsigned int _id;
    TextureType _type;

    void load_image(const std::string& file_path);
};

class TexDict {
   public:
    static Texture init_texture(std::string path, TextureType type);
    TexDict() = delete;

   private:
    static std::unordered_map<std::string, unsigned int> dict;
};