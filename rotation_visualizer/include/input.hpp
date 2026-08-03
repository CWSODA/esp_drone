#include <GLFW/glfw3.h>
#include "serial_port.hpp"
#include "camera.hpp"

constexpr float P_inc = 0.5f;

// mouse
float mouse_lastX = 400;
float mouse_lastY = 300;
float x_offset = 0, y_offset = 0;
bool is_first_mouse = true;
float mouse_sensitivity = 0.05;
bool is_cursor = false;

struct InputData {
    float P;
    float delta_time;
    Camera camera;
};

class TapKey {
   public:
    TapKey(int key) { key_ = key; }

    bool is_tapped(GLFWwindow* window) {
        bool state = glfwGetKey(window, key_) == GLFW_PRESS;

        if (!was_on && state) {
            was_on = true;
            return true;
        }
        if (was_on && !state) {
            was_on = false;
        }
        return false;
    }

   private:
    int key_;
    bool was_on = false;
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (is_first_mouse) {
        is_first_mouse = false;
    } else if (!is_cursor) {
        x_offset = xpos - mouse_lastX;
        y_offset = mouse_lastY - ypos;
    }

    mouse_lastX = xpos;
    mouse_lastY = ypos;
}

void process_input(GLFWwindow* window, SerialPort& sp, InputData& data) {
    if (x_offset != 0 || y_offset != 0) {
        data.camera.process_mouse_movement(x_offset, y_offset);
        x_offset = 0;
        y_offset = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    static TapKey downkey(GLFW_KEY_DOWN);
    static TapKey upkey(GLFW_KEY_UP);
    if (downkey.is_tapped(window)) {
        if (data.P > 0) {
            data.P -= P_inc;
            char msg[20];
            snprintf(msg, sizeof(msg), "%f\n", data.P);
            std::string str = std::string(msg);
            sp.write(str);
        }
    }
    if (upkey.is_tapped(window)) {
        data.P += P_inc;
        char msg[20];
        snprintf(msg, sizeof(msg), "%f\n", data.P);
        std::string str = std::string(msg);
        sp.write(str);
    }

    bool is_forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool is_backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool is_left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool is_right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool is_shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    bool is_up = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool is_down = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    data.camera.process_keys(is_forward, is_backward, is_left, is_right, is_shift, is_up, is_down,
                             data.delta_time);

    // toggles
    static bool is_wireframe = false;
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