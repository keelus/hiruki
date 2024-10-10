#ifndef HIRUKI_MATH_VECTOR3_H
#define HIRUKI_MATH_VECTOR3_H

#include <cmath>
#include <ostream>
#include <stdexcept>

namespace Hiruki {
	namespace Math {
		class Vector2;
		class Vector4;

		class Vector3 {
		public:
			inline Vector3 (float x, float y, float z) : x(x), y(y), z(z) {}
			constexpr Vector3 () : x(0), y(0), z(0) {}
		
			Vector3 (const Vector2 &that);
			Vector3 (const Vector4 &that);
		
			float x, y, z;
		
			static constexpr Vector3 zero() {
				return Vector3();
			}
		
			static constexpr Vector3 one() {
				Vector3 vector;
				vector.x = 1;
				vector.y = 1;
				vector.z = 1;
				return vector;
			}
		
			static constexpr Vector3 up() {
				Vector3 vector;
				vector.y = 1;
				return vector;
			}
		
			static constexpr Vector3 forward() {
				Vector3 vector;
				vector.z = 1;
				return vector;
			}
		
			static constexpr Vector3 right() {
				Vector3 vector;
				vector.x = 1;
				return vector;
			}
		
			inline Vector3 add(const Vector3 &that) const {
				return Vector3 (
					this->x + that.x,
					this->y + that.y,
					this->z + that.z
				);
			}
		
			inline Vector3 sub(const Vector3 &that) const {
				return Vector3 (
					this->x - that.x,
					this->y - that.y,
					this->z - that.z
				);
			}
		
			inline float dot(const Vector3 &that) const {
				return this->x * that.x + this->y * that.y + this->z * that.z;
			}
		
			inline Vector3 mul(float factor) const {
				return Vector3 (
					this->x * factor,
					this->y * factor,
					this->z * factor
				);
			}
		
			inline Vector3 div(float factor) const {
				if(factor == 0) {
					throw std::invalid_argument("Vector3 division by zero.");
				}
				return Vector3 (
					this->x / factor,
					this->y / factor,
					this->z / factor
				);
			}
		
			inline Vector3 cross(const Vector3 &that) const {
				return Vector3 (
					this->y * that.z - this->z * that.y,
					this->z * that.x - this->x * that.z,
					this->x * that.y - this->y * that.x
				);
			}
		
			inline float length() const {
				return std::sqrt(
					this->x * this->x +
					this->y * this->y +
					this->z * this->z
				);
			}
		
			inline Vector3 normalized() const {
				float length = this->length();
				if(length == 0)
					throw std::invalid_argument("Cannot normalize a zero length Vector3.");
				return this->div(length);
			}

			inline Vector3 lerp(const Vector3 &that, float t) const {
				return Vector3 (
					std::lerp(this->x, that.x, t),
					std::lerp(this->y, that.y, t),
					std::lerp(this->z, that.z, t)
				);
			}
		
			friend std::ostream& operator<<(std::ostream& os, const Vector3 &vec) {
				os << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
				return os;
			}
		};
		
		class Vector3i {
			public:
				inline Vector3i (int x, int y, int z) : x(x), y(y), z(z) {}
				constexpr Vector3i () : x(0), y(0), z(0) {}
		
				Vector3i (const Vector2 &that);
				Vector3i (const Vector3 &that);

				int x, y, z;
		};
	}
}

#endif
