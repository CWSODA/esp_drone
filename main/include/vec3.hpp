#pragma once

#include <stdio.h>
#include <math.h>

class Vec3 {
   public:
    float x = 0, y = 0, z = 0;
    static Vec3 zeros() { return Vec3(0, 0, 0); }

    float& at(size_t n) {
        if (n == 0) return x;
        if (n == 1) return y;
        if (n == 2) return z;

        printf("INVALID INDEX FOR VEC3\n");
        return x;  // default option
    }

    void operator+=(const Vec3& other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
    }
    Vec3 operator+(const Vec3& other) const {
        Vec3 out = *this;
        out += other;
        return out;
    }

    void operator-=(const Vec3& other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
    }

    Vec3 operator-() const { return Vec3::zeros() - *this; }

    Vec3 operator-(const Vec3& other) const {
        Vec3 out = *this;
        out -= other;
        return out;
    }

    Vec3 operator*(const Vec3& other) const {
        Vec3 out{
            .x = this->x * other.x,
            .y = this->y * other.y,
            .z = this->z * other.z,
        };
        return out;
    }

    Vec3 operator*(const float mult) const {
        Vec3 out{
            .x = this->x * mult,
            .y = this->y * mult,
            .z = this->z * mult,
        };
        return out;
    }

    float len() const { return sqrtf(x * x + y * y + z * z); }

    Vec3 normalize() const { return (*this) * (1.0f / this->len()); }

    Vec3 cross(const Vec3& other) const {
        // x  y  z
        // x1 y1 z1
        // x2 y2 z2
        return Vec3{
            .x = y * other.z - z * other.y,
            .y = z * other.x - x * other.z,
            .z = x * other.y - y * other.x,
        };
    }

    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    void disp(const char* label = nullptr) const {
        if (label == nullptr)
            printf("%f,%f,%f\n", x, y, z);
        else
            printf("%s: %f, %f, %f\n", label, x, y, z);
    }

    void disp(const char label) const {
        printf("%c:%.3f,%.3f,%.3f\n", label, x, y, z);
    }
};