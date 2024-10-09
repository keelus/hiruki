#ifndef HIRUKI_MATH_VECTOR4_H
#define HIRUKI_MATH_VECTOR4_H

#include <cmath>
#include <ostream>
#include <stdexcept>

namespace Hiruki {
	namespace Math {
		class Vector2;
		class Vector3;

		class Vector4 {
		public:
			inline Vector4 (float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
			inline Vector4 (float x, float y, float z) : x(x), y(y), z(z), w(1) {}
			constexpr Vector4 () : x(0), y(0), z(0), w(1) {}
		
			Vector4 (const Vector2 &that);
			Vector4 (const Vector3 &that);
		
			float x, y, z, w;
		
			static constexpr Vector4 zero() {
				return Vector4();
			}
		
			static constexpr Vector4 one() {
				Vector4 vector;
				vector.x = 1;
				vector.y = 1;
				vector.z = 1;
				vector.w = 1;
				return vector;
			}
		
			inline Vector4 add(const Vector4 &that) const {
				return Vector4 (
					this->x + that.x,
					this->y + that.y,
					this->z + that.z,
					this->w + that.w
				);
			}
		
			inline Vector4 sub(const Vector4 &that) const {
				return Vector4 (
					this->x - that.x,
					this->y - that.y,
					this->z - that.z,
					this->w - that.w
				);
			}
		
			inline float dot(const Vector4 &that) const {
				return this->x * that.x + this->y * that.y + this->z * that.z + this->w * that.w;
			}
		
			inline Vector4 mul(float factor) const {
				return Vector4 (
					this->x * factor,
					this->y * factor,
					this->z * factor,
					this->w * factor
				);
			}
		
			inline Vector4 div(float factor) const {
				if(factor == 0) {
					throw std::invalid_argument("Vector4 division by zero.");
				}
				return Vector4 (
					this->x / factor,
					this->y / factor,
					this->z / factor,
					this->w / factor
				);
			}
		
			inline float length() const {
				return std::sqrt(
					this->x * this->x +
					this->y * this->y +
					this->z * this->z +
					this->w * this->w
				);
			}
		
			inline Vector4 normalized() const {
				float length = this->length();
				if(length == 0)
					throw std::invalid_argument("Cannot normalize a zero length Vector4.");
				return this->div(length);
			}

			inline Vector4 lerp(const Vector4 &that, float t) const {
				return Vector4 (
					std::lerp(this->x, that.x, t),
					std::lerp(this->y, that.y, t),
					std::lerp(this->z, that.z, t),
					std::lerp(this->w, that.w, t)
				);
			}
		
			inline Vector4 perspectiveDivide() const {
				if(this->w == 0) {
					return *this;
				}
		
				return Vector4 (
					this->x / this->w,
					this->y / this->w,
					this->z / this->w,
					this->w
				);
			}
		
			friend std::ostream& operator<<(std::ostream& os, const Vector4 &vec) {
				os << "Vector4(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
				return os;
			}
		};
	}
}

#endif
