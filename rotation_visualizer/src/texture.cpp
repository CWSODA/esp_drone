#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "log.hpp"

Texture::Texture(const std::string& file_path, TextureType type) {
    log_msg("Generating ID");
    glGenTextures(1, &_id);
    log_msg("Generated ID: ", _id);

    int prev_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);

    glBindTexture(GL_TEXTURE_2D, _id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    load_image(file_path);
    _type = type;

    glBindTexture(GL_TEXTURE_2D, prev_texture);
}

Texture::Texture(unsigned int id, TextureType type) {
    _id = id;
    _type = type;
}

void Texture::bind(unsigned int texture_unit) {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::set_wrap_type(WrapType type) {
    int prev_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);

    GLenum wrap_type = static_cast<GLenum>(type);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_type);
    glBindTexture(GL_TEXTURE_2D, prev_texture);
}
void Texture::set_filter_type(FilterType type) {
    int prev_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);

    GLenum filter_type = static_cast<GLenum>(type);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_type);
    glBindTexture(GL_TEXTURE_2D, prev_texture);
}

// Texture::~Texture() { glDeleteTextures(1, &_id); }

void Texture::load_image(const std::string& texture_path) {
    std::cout << "loading image: " << texture_path << std::endl;
    int img_width, img_height, nChannels;
    unsigned char* data =
        stbi_load(texture_path.c_str(), &img_width, &img_height, &nChannels, 0);

    if (data) {
        GLenum format = (nChannels == 4) ? GL_RGBA : GL_RGB;
        // texture type, mipmap level, store type, width/height, must be 0,
        // input format, input data type, actual data
        glTexImage2D(GL_TEXTURE_2D, 0, format, img_width, img_height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture from: " << texture_path
                  << std::endl;
    }
    stbi_image_free(data);
}

std::unordered_map<std::string, unsigned int> TexDict::dict;
Texture TexDict::init_texture(std::string path, TextureType type) {
    auto ptr = dict.find(path);
    if (ptr != dict.end()) {
        return Texture(ptr->second, type);
    }

    Texture texture(path, type);
    dict[path] = texture.get_id();

    return texture;
}