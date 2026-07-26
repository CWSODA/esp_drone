#include <math.h>

#include "vec3.hpp"

constexpr float RAD2DEG = 180.0f / 3.1415f;

struct Quat {
   public:
    float w, x, y, z;

    Quat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
    Quat(float x, float y, float z) : x(x), y(y), z(z) {}
    Quat() {}

    // used to initiate gyroscope quaternion
    static Quat from_data(Vec3 accel, Vec3 mag) {
        Vec3 up = accel.normalize();

        // only perpendicular component
        Vec3 north = (mag - up * mag.dot(up)).normalize();
        Vec3 west = up.cross(north).normalize();

        up.disp("up");
        north.disp("north");
        west.disp("west");
        float rot_mat[3][3] = {
            {north.x, west.x, up.x},
            {north.y, west.y, up.y},
            {north.z, west.z, up.z},
        };

        float trace = rot_mat[0][0] + rot_mat[1][1] + rot_mat[2][2];
        float w = 0.5f * sqrtf(1 + trace);
        float x = (rot_mat[2][1] - rot_mat[1][2]) / (4.0f * w);
        float y = (rot_mat[0][2] - rot_mat[2][0]) / (4.0f * w);
        float z = (rot_mat[1][0] - rot_mat[0][1]) / (4.0f * w);

        // float trace = north.x + east.y + down.z;
        // float w = 0.5f * sqrtf(1 + trace);
        // float x = (east.z - down.y) / (4.0f * w);
        // float y = (down.x - north.z) / (4.0f * w);
        // float z = (north.y - east.x) / (4.0f * w);
        printf("Trace: %f\n", trace);

        // accel.disp("Original Accel");
        // down.disp("Down");
        // mag.disp("Original Mag");
        // north.disp("North");
        Quat out = Quat(w, x, y, z);
        out.disp("init quat");
        return out;
    }

    void operator+=(const Quat& other) {
        w += other.w;
        x += other.x;
        y += other.y;
        z += other.z;
    }

    Quat operator+(const Quat& other) {
        Quat out = *this;
        out += other;
        return out;
    }

    Quat operator*(const Quat& other) const {
        Quat out;
        // (w + xi + yj + zk) * (w' + x'i + y'j + z'k)
        // = w*w' + w*x'i + w*y'j + w*z'k
        // + xi*w' + xi*x'i + xi*y'j + xi*z'k
        // + yj*w' + yj*x'i + yj*y'j + yj*z'k
        // + zk*w' + zk*x'i + zk*y'j + zk*z'k
        out.w = w * other.w - x * other.x - y * other.y - z * other.z;
        out.x = w * other.x + x * other.w + y * other.z - z * other.y;
        out.y = w * other.y - x * other.z + y * other.w + z * other.x;
        out.z = w * other.z + x * other.y - y * other.x + z * other.w;
        return out;
    }

    Quat operator*(const float mult) const {
        return Quat(w * mult, x * mult, y * mult, z * mult);
    }

    Quat conjugate() const { return Quat(w, -x, -y, -z); }

    float len() const { return sqrtf(w * w + x * x + y * y + z * z); }

    Quat normalize() const { return (*this) * (1.0f / len()); }

    Vec3 to_vec() const { return Vec3{x, y, z}; }

    void disp_angle_vec() const {
        // w = cos theta/2
        float theta = acosf(w) * 2 * RAD2DEG;
        Vec3 r_axis = this->to_vec().normalize();
        printf("Theta: %f, Rotation Axis: %f, %f, %f\n", theta, r_axis.x,
               r_axis.y, r_axis.z);
    }

    // prints quaternion values seperated with commas
    // has option to pass in a label
    void disp(const char* label = nullptr) const {
        if (label == nullptr)
            printf("%f,%f,%f,%f\n", w, x, y, z);
        else
            printf("%s: %f, %f, %f, %f\n", label, w, x, y, z);
    }
};