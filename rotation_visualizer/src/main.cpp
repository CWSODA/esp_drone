#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "lib.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "cube.hpp"
#include "model.hpp"
#include "mesh.hpp"
#include "plane.hpp"
#include "border.hpp"

#include "serial_port.hpp"
#include "parser.hpp"

#define WIDTH 800
#define HEIGHT 600

const float mix_increment = 0.005;
float mix = 0.5f;

int win_width, win_height;

// timings
float delta_time = 0;
float last_frame = 0;

// mouse
float mouse_lastX = 400;
float mouse_lastY = 300;
bool is_first_mouse = true;
float mouse_sensitivity = 0.05;

#define ENABLE_SP true

Camera camera{};

int main() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW!");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(WIDTH, HEIGHT, "StartingOpenGL", NULL, NULL);

    if (window == NULL) {
        const char* description;
        int code = glfwGetError(&description);
        printf("GLFW Error: %s (Code %d)\n", description, code);
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);  // set this window as current context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    // callbacks and settings
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGL();

    /* END OF WINDOW CREATION */

    // Vertex Array Obj, Vertex Buffer Obj, Element Buffer Obj
    unsigned int cube_VAO, cube_VBO;
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);
    glGenBuffers(1, &cube_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    // position
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_norm_cube), tex_norm_cube,
                 GL_STATIC_DRAW);

    // index, vector size, type, should normalize, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // LOAD SHADERS
    Shader solid_shader("../src/shaders/vs/light.vert",
                        "../src/shaders/fs/solid.frag");

    Shader grid_shader("../src/shaders/vs/pos_only.vert",
                       "../src/shaders/fs/grid.frag");

    // camera settings
    camera.position = glm::vec3(0.0f, 1.0f, 4.0f);
    camera.camera_type = CAMERA_TYPE::FLY;

    // Resize window properly
    glfwGetFramebufferSize(window, &win_width, &win_height);
    glViewport(0, 0, win_width, win_height);

    // Enable tests
    glEnable(GL_DEPTH_TEST);

    Mesh plane_mesh = get_plane_mesh();
    glm::vec3 clear_color(0.1f, 0.1f, 0.1f);

#if ENABLE_SP
    SerialPort sp;
    sp.try_auto_connect();
    sp.pull_buffer();
#endif

    while (!glfwWindowShouldClose(window)) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        process_input(window);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);  // on by default

        // get matrices
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection =
            glm::perspective(glm::radians(camera.fov),
                             (float)win_width / win_height, 0.1f, 1000.0f);

        // GRID
        grid_shader.use();
        grid_shader.set_mat4("view", view);
        grid_shader.set_mat4("projection", projection);
        glm::mat4 plane_model = glm::mat4(1.0);
        plane_model = glm::scale(plane_model, glm::vec3(20.0f, 1.0f, 20.0f));
        plane_model = glm::translate(plane_model, glm::vec3(0, -6, 0));
        grid_shader.set_mat4("model", plane_model);
        grid_shader.set_float("grid_size", 1.0f);
        grid_shader.set_float("line_width", 0.02f);
        plane_mesh.draw(grid_shader);

        // parse
        static glm::quat q;
#if ENABLE_SP
        sp.try_auto_connect();
        parse(sp, q);
        printf("%f, %f, %f, %f\n", q.w, q.x, q.y, q.z);
#endif

        // CUBES
        float side = 0.05f;
        float length = 2.0f;
        glm::mat4 base = glm::mat4_cast(q);
        glm::mat4 block_models[] = {
            glm::scale(glm::translate(base, glm::vec3(length / 2, 0, 0)),
                       glm::vec3(length, side, side)),
            glm::scale(glm::translate(base, glm::vec3(0, length / 2, 0)),
                       glm::vec3(side, length, side)),
            glm::scale(glm::translate(base, glm::vec3(0, 0, length / 2)),
                       glm::vec3(side, side, length)),
        };
        glm::vec3 colors[] = {
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 0, 1),
        };

        glBindVertexArray(cube_VAO);
        for (int idx = 0; idx < 3; idx++) {
            glm::mat4 block_model = block_models[idx];
            glm::vec3 color = colors[idx];

            solid_shader.use();
            solid_shader.set_mat4("view", view);
            solid_shader.set_mat4("model", block_model);
            solid_shader.set_mat4("projection", projection);
            solid_shader.set_vec3("color", color);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteBuffers(1, &cube_VBO);

    glfwTerminate();

    return 0;
}

void set_lights(Shader& shader) {}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    win_width = width;
    win_height = height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float x_offset = 0, y_offset = 0;
    if (is_first_mouse) {
        is_first_mouse = false;
    } else {
        x_offset = xpos - mouse_lastX;
        y_offset = mouse_lastY - ypos;
    }

    mouse_lastX = xpos;
    mouse_lastY = ypos;

    camera.process_mouse_movement(x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    camera.process_mouse_scroll(y_offset);
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (mix > 0) {
            mix -= mix_increment;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (mix < 1) {
            mix += mix_increment;
        }
    }

    bool is_forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool is_backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool is_left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool is_right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool is_shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    bool is_up = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool is_down = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    camera.process_keys(is_forward, is_backward, is_left, is_right, is_shift,
                        is_up, is_down, delta_time);

    // toggles
    static bool is_wireframe = false;
    static bool is_cursor = false;
    static bool last_z = false;
    static bool last_c = false;

    bool current_z = glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;
    bool current_c = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;

    if (!last_z && current_z) {
        is_wireframe = !is_wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, is_wireframe ? GL_LINE : GL_FILL);
    }
    if (!last_c && current_c) {
        is_cursor = !is_cursor;
        is_first_mouse = true;
        glfwSetInputMode(window, GLFW_CURSOR,
                         is_cursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    last_z = current_z;
    last_c = current_c;
}