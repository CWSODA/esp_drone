#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CAMERA_TYPE { GROUNDED, FLY, PLANE };

class Camera {
   public:
    // camera vectors
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 up, right, front;

    // angles
    float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;

    // speed
    float cam_speed = 2.5f, mouse_sensitivity = 0.05f, zoom_sensitivity = 3.0f,
          speed_boost = 3.5f;
    float fov_max = 110.0f, fov_min = 5.0f;
    float fov = 90.0f;

    // type
    CAMERA_TYPE camera_type = CAMERA_TYPE::FLY;

    Camera(glm::vec3 cam_pos) : position(cam_pos) { update_camera_vectors(); }
    Camera(void) { update_camera_vectors(); }

    glm::mat4 get_view_matrix() {
        return glm::lookAt(position, position + front, up);
    }

    void process_mouse_movement(float x_offset, float y_offset) {
        x_offset *= mouse_sensitivity;
        y_offset *= mouse_sensitivity;

        yaw += x_offset;
        pitch += y_offset;

        if (pitch > 89) pitch = 89;
        if (pitch < -89) pitch = -89;
        if (yaw > 360) yaw -= 360;
        if (yaw < -360) yaw += 360;

        update_camera_vectors();
    }
    void process_mouse_scroll(float y_offset) {
        fov -= y_offset * zoom_sensitivity;
        if (fov > fov_max) fov = fov_max;
        if (fov < fov_min) fov = fov_min;
    }
    void process_keys(bool is_forward, bool is_backward, bool is_left,
                      bool is_right, bool is_run, bool is_up, bool is_down,
                      float delta_time) {
        glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

        velocity.x = (is_right ? 1 : 0) + (is_left ? -1 : 0);
        velocity.y = (is_forward ? 1 : 0) + (is_backward ? -1 : 0);
        velocity.z = (is_up ? 1 : 0) + (is_down ? -1 : 0);

        if (glm::length(velocity) == 0) return;

        velocity = glm::normalize(velocity);
        velocity *= (cam_speed + (is_run ? speed_boost : 0)) * delta_time;

        if (camera_type == CAMERA_TYPE::PLANE) {
            return;
        }
        if (camera_type == CAMERA_TYPE::FLY) {
            glm::vec3 grounded_front =
                glm::normalize(glm::cross(world_up, right));

            position += right * velocity.x;
            position += grounded_front * velocity.y;
            position += world_up * velocity.z;

            return;
        }
        if (camera_type == CAMERA_TYPE::GROUNDED) {
            glm::vec3 grounded_front =
                glm::normalize(glm::cross(world_up, right));

            velocity.z = 0;
            velocity = glm::normalize(velocity);  // remove the upward component

            position += grounded_front * velocity.y;
            position += right * velocity.x;

            return;
        }
    }

   private:
    void update_camera_vectors() {
        front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }

    glm::mat4 look_at(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
        glm::vec3 z_axis = glm::normalize(position - target);
        glm::vec3 x_axis = glm::normalize(glm::cross(up, z_axis));

        glm::mat4 rotation_mat(x_axis.x, x_axis.y, x_axis.z, 0,  //
                               up.x, up.y, up.z, 0,              //
                               z_axis.x, z_axis.y, z_axis.z, 0,  //
                               0, 0, 0, 1                        //
        );

        glm::mat4 translation_mat(1, 0, 0, -position.x,  //
                                  0, 1, 0, -position.y,  //
                                  0, 0, 1, -position.z,  //
                                  0, 0, 0, 1             //
        );

        // constructor makes a transpose of the display >:(
        return glm::transpose(rotation_mat) * glm::transpose(translation_mat);
    }
};
