#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shader.hpp"

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

    void draw(glm::vec3 a, glm::vec3 b) {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        float verts[] = {a.x, a.y, a.z, b.x, b.y, b.z};
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

        glLineWidth(10.0f);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    };

   private:
    unsigned int vao, vbo;
};