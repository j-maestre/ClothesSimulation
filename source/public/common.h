#pragma once

#include "math.h"
#include <assert.h>

namespace JE{

    struct Vec3 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        __forceinline Vec3 operator-(const Vec3& other) {
            return { x - other.x, y - other.y, z - other.z };
        }

        __forceinline Vec3 operator+(const Vec3& other) {
            return { x + other.x, y + other.y, z + other.z };
        }

        __forceinline Vec3 operator+=(const Vec3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        __forceinline Vec3 operator*(float scalar) const {
            return { x * scalar, y * scalar, z * scalar };
        }

        __forceinline Vec3 operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        __forceinline Vec3 operator/(float scalar) const {
            return { x / scalar, y / scalar, z / scalar };
        }

        __forceinline float operator[](int index) {
            assert((index >= 0 && index <= 2) && "index out of bounds");
            switch (index) {
                case 0: return x; break;
                case 1: return y; break;
                case 2: return z; break;
            }

        }

        __forceinline float Dot(const Vec3& other) const {
            return (x * other.x) + (y * other.y) + (z * other.z);
        }

        __forceinline Vec3 Cross(const Vec3& other) const {
            return {
                (y * other.z - z * other.y),
                (z * other.x - x * other.z),
                (x * other.y - y * other.x)
            };
        }

        Vec3 Normalize() const {
            float magnitude = sqrtf(x * x + y * y + z * z);
            if (magnitude > 0.0f) {
                return { x / magnitude, y / magnitude, z / magnitude };
            }
            return { 0.0f, 0.0f, 0.0f };
        }

        __forceinline float Length() const {
            return sqrt(Dot(*this));
        }
    };

    struct Vec2 {
        float x = 0.0f;
        float y = 0.0f;

        __forceinline Vec2 operator-(const Vec2& other) {
            return { x - other.x, y - other.y };
        }

        __forceinline Vec2 operator+(const Vec2& other) {
            return { x + other.x, y + other.y };
        }

        __forceinline Vec2 operator+=(const Vec2& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        __forceinline Vec2 operator*(float scalar) const {
            return { x * scalar, y * scalar };
        }

        __forceinline Vec2 operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        __forceinline Vec2 operator/(float scalar) const {
            return { x / scalar, y / scalar };
        }

        __forceinline float Dot(const Vec2& other) const {
            return (x * other.x) + (y * other.y);
        }

        __forceinline float Length() const {
            return sqrt(Dot(*this));
        }
    };

    struct Point {
        Vec3 position;
        Vec3 previous_position;
        float mass = 1.0f;
        float friction_factor = 1.0f;
        bool fixed = false;

    };

    struct Vertex{
        Vec3 pos;
        Vec3 normal;
        Vec2 uv;
    };
}