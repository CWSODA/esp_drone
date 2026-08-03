#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

// IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "lib.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "cube.hpp"
#include "model.hpp"
#include "mesh.hpp"
#include "plane.hpp"
#include "border.hpp"
#include "line.hpp"

#include "serial_port.hpp"
#include "parser.hpp"
#include "input.hpp"

#define WIDTH 800
#define HEIGHT 600

int win_width, win_height;

// timings
float last_frame = 0;

#define ENABLE_SP true

// includes camera, delta_time
InputData input_data;
Camera& camera = input_data.camera;

int main() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW!");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "StartingOpenGL", NULL, NULL);

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& imgui_io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    /* END OF WINDOW CREATION */

    // Vertex Array Obj, Vertex Buffer Obj, Element Buffer Obj
    unsigned int cube_VAO, cube_VBO;
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);
    glGenBuffers(1, &cube_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    // position
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_norm_cube), tex_norm_cube, GL_STATIC_DRAW);

    // index, vector size, type, should normalize, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // LOAD SHADERS
    Shader solid_shader("../src/shaders/vs/3d.vert", "../src/shaders/fs/single_color.frag");

    Shader grid_shader("../src/shaders/vs/pos_only.vert", "../src/shaders/fs/grid.frag");

    Liner liner(cube_VAO, cube_VBO, solid_shader);

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
        input_data.delta_time = current_frame - last_frame;
        last_frame = current_frame;

        process_input(window, sp, input_data);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);  // on by default

        // get matrices
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov),
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
        static glm::quat q = glm::quat(1, 0, 0, 0);
        static glm::vec3 accel;
        static glm::vec3 mag;
#if ENABLE_SP
        sp.try_auto_connect();
        parse(sp, q, accel, mag);
        accel = glm::normalize(accel);
        mag = glm::normalize(mag);
        // disp(q, "Quat");
#endif

        // cardinal directions and colors
        glm::vec3 dirs[] = {
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 0, 1),
        };
        glm::vec3 colors[] = {
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 0, 1),
        };
        for (int idx = 0; idx < 3; idx++) {
            // rotate and draw
            liner.draw(gl_space(q * dirs[idx]), view, projection, colors[idx]);
        }

        liner.draw(gl_space(accel * 2.0f), view, projection, glm::vec3(1, 1, 0), 0.02f);
        liner.draw(gl_space(mag * 2.0f), view, projection, glm::vec3(1, 0, 1), 0.02f);

        /* ------------------------ IMGUI ----------------------- */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static float mahony_P = 1.0f;
        static float mahony_mag_weight = 1.0f;
        if (ImGui::Begin("Mahony PI Settings")) {
            if (ImGui::SliderFloat("P", &mahony_P, 0.0f, 100.0f, "%.4f")) {
                // make sure PID values are non-negative
                mahony_P = std::max(mahony_P, 0.0f);
                sp.write_float(mahony_P, 'p');
            }
            if (ImGui::SliderFloat("Mag Weight", &mahony_mag_weight, 0.0f, 1.0f, "%.4f")) {
                // make sure PID values are non-negative
                mahony_mag_weight = std::max(mahony_mag_weight, 0.0f);
                sp.write_float(mahony_mag_weight, 'm');
            }
            ImGui::End();
        }

        // IMGUI RENDER
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    camera.process_mouse_scroll(y_offset);
}