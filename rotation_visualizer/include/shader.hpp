#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
   private:
    unsigned int ID;

   public:
    Shader(const char* vertexPath, const char* fragmentPath,
           const char* geomPath = nullptr) {
        std::string vertex_code, frag_code;
        std::ifstream v_file, f_file;
        v_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        f_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            v_file.open(vertexPath);
            f_file.open(fragmentPath);
            // buffer file content into streams
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << v_file.rdbuf();
            fShaderStream << f_file.rdbuf();
            // close files
            v_file.close();
            f_file.close();
            // convert streams into strings
            vertex_code = vShaderStream.str();
            frag_code = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::FAILED TO READ SHADER FILE: " << std::endl;
        }
        const char* v_shader_code = vertex_code.c_str();
        const char* f_shader_code = frag_code.c_str();

        unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        int success;
        char infoLog[512];

        glShaderSource(vert_shader, 1, &v_shader_code, NULL);
        glCompileShader(vert_shader);
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
        glShaderSource(frag_shader, 1, &f_shader_code, NULL);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vert_shader);
        glAttachShader(ID, frag_shader);

        // optional geometry shader
        unsigned int geom_shader = -1;
        if (geomPath != nullptr) {
            std::string geom_code;
            std::ifstream g_file;
            g_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                g_file.open(geomPath);
                // buffer file content into streams
                std::stringstream g_shader_stream;
                g_shader_stream << g_file.rdbuf();
                // close files
                g_file.close();
                // convert streams into strings
                geom_code = g_shader_stream.str();
            } catch (std::ifstream::failure e) {
                std::cout << "ERROR::FAILED TO READ SHADER FILE: " << std::endl;
            }
            const char* g_shader_code = geom_code.c_str();

            geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
            int success;
            char infoLog[512];
            glShaderSource(geom_shader, 1, &g_shader_code, NULL);
            glCompileShader(geom_shader);
            glGetShaderiv(geom_shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(geom_shader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n"
                          << infoLog << std::endl;
            }
            glAttachShader(ID, geom_shader);
        }

        /* -------------------- Final Linking ------------------- */
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }

        /* -------------- Delete Unbounded Shaders -------------- */
        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
        if (geom_shader != -1) glDeleteShader(geom_shader);
    }

    // activate shader
    void use() { glUseProgram(ID); }

    // change uniform variable values
    void set_bool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void set_int(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void set_float(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void set_mat4(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                           glm::value_ptr(value));
    }

    void set_vec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void set_vec3(const std::string& name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
};