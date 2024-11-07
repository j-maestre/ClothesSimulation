#pragma once
#include "math.h"

namespace JE {

	struct Vec3 {
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		Vec3 operator-(const Vec3& other) {
			return { x - other.x, y - other.y, z - other.z };
		}

		Vec3 operator+(const Vec3& other) {
			return { x + other.x, y + other.y, z + other.z };
		}

		Vec3 operator+=(const Vec3& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vec3 operator*(float scalar) const {
			return { x * scalar, y * scalar, z * scalar };
		}

		Vec3 operator*=(float scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		Vec3 operator/(float scalar) const {
			return { x / scalar, y / scalar, z / scalar };
		}

		float Dot(const Vec3& other) const {
			return (x * other.x) + (y * other.y) + (z * other.z);
		}

		float Length() const {
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
}