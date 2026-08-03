#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shader.hpp"

class Liner {
   public:
    Liner(unsigned int vao, unsigned int vbo, Shader shader)
        : shader_(shader), vao_(vao), vbo_(vbo) {}

    void draw(const glm::vec3 dir_vec, const glm::mat4& view,
              const glm::mat4& projection, const glm::vec3 color,
              const float thickness = 0.05f) {
        this->draw(glm::vec3(0, 0, 0), dir_vec, view, projection, color,
                   thickness);
    }
    void draw(const glm::vec3 a, const glm::vec3 b, const glm::mat4& view,
              const glm::mat4& projection, const glm::vec3 color,
              const float thickness = 0.05f) {
        shader_.use();
        shader_.set_mat4("view", view);
        shader_.set_mat4("projection", projection);
        shader_.set_vec3("color", color);

        glm::vec3 dir = b - a;
        float len = glm::length(dir);
        float r = sqrt(dir.x * dir.x + dir.y * dir.y);

        // scale, translate out, rotate pitch, rotate heading, translate
        glm::mat4 model(1.0f);
        model = glm::translate(model, a);
        model = glm::rotate(model, atan2(dir.y, dir.x), glm::vec3(0, 0, 1));
        model = glm::rotate(model, atan2(-dir.z, r), glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(len / 2, 0, 0));
        model = glm::scale(model, glm::vec3(len, thickness, thickness));
        shader_.set_mat4("model", model);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        float scale = 0.2;
        shader_.set_mat4("model", glm::scale(glm::translate(glm::mat4(1.0), a),
                                             glm::vec3(scale)));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        shader_.set_mat4("model", glm::scale(glm::translate(glm::mat4(1.0), b),
                                             glm::vec3(scale)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
    };

   private:
    unsigned int vao_, vbo_;
    Shader shader_;
};

class LineRenderer {
   public:
    LineRenderer() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, nullptr,
                     GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3,
                              (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void draw(const glm::vec3 a, const glm::vec3 b, const glm::mat4& view,
              const glm::mat4& projection, const glm::vec3 color) {
        line_shader.use();
        line_shader.set_mat4("view", view);
        line_shader.set_mat4("projection", projection);
        line_shader.set_mat4("model", glm::mat4(1.0f));
        line_shader.set_vec3("color", glm::vec3(1, 0, 0));
        line_shader.set_vec3("dir", b - a);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        float verts[] = {a.x, a.y, a.z, b.x, b.y, b.z};
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

        // glDrawArrays(GL_LINES, 0, 2);
        glPointSize(50.0f);
        glDrawArrays(GL_POINTS, 0, 2);
        // glDrawArrays(GL_TRIANGLE_STRIP, 0, 2);
        glBindVertexArray(0);
    };

   private:
    unsigned int vao, vbo;
    Shader line_shader = Shader("../src/shaders/vs/3d.vert",
                                "../src/shaders/fs/single_color.frag",
                                "../src/shaders/geom/thick_line.geom");
};