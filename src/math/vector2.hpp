#ifndef HIRUKI_MATH_VECTOR2_H
#define HIRUKI_MATH_VECTOR2_H

#include <cmath>
#include <ostream>
#include <stdexcept>

namespace Hiruki {
	namespace Math {
		class Vector3;
		class Vector4;

		class Vector2 {
		public:
			inline Vector2 (float x, float y) : x(x), y(y) {}
			constexpr Vector2 () : x(0), y(0) {}
		
			Vector2(const Vector3 &that);
			Vector2(const Vector4 &that);
		
			float x, y;
		
			static constexpr Vector2 zero() {
				return Vector2();
			}
		
			static constexpr Vector2 one() {
				Vector2 vector;
				vector.x = 1;
				vector.y = 1;
				return vector;
			}
		
			inline Vector2 add(const Vector2 &that) const {
				return Vector2 (
					this->x + that.x,
					this->y + that.y
				);
			}
		
			inline Vector2 sub(const Vector2 &that) const {
				return Vector2 (
					this->x - that.x,
					this->y - that.y
				);
			}
		
			inline float dot(const Vector2 &that) const {
				return this->x * that.x + this->y * that.y;
			}
		
			inline Vector2 mul(float factor) const {
				return Vector2 (
					this->x * factor,
					this->y * factor
				);
			}
		
			inline Vector2 div(float factor) const {
				if(factor == 0) {
					throw std::invalid_argument("Vector2 division by zero.");
				}
				return Vector2 (
					this->x / factor,
					this->y / factor
				);
			}
		
			inline float cross(const Vector2 &that) const {
				return this->x * that.y - this->y * that.x;
			}
		
			inline float length() const {
				return std::sqrt(
					this->x * this->x +
					this->y * this->y
				);
			}
		
			inline Vector2 normalized() const {
				float length = this->length();
				if(length == 0)
					throw std::invalid_argument("Cannot normalize a zero length Vector2.");
				return this->div(length);
			}

			inline Vector2 lerp(const Vector2 &that, float t) const {
				return Vector2 (
					std::lerp(this->x, that.x, t),
					std::lerp(this->y, that.y, t)
				);
			}

			inline static float edgeCross(const Vector2 &a, const Vector2 &b, const Vector2 &p) {
				return p.sub(a).cross(b.sub(a));
			}
		
			friend std::ostream& operator<<(std::ostream& os, const Vector2 &vec) {
				os << "Vector2(" << vec.x << ", " << vec.y << ")";
				return os;
			}
		};
	}
}

#endif
