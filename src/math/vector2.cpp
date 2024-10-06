#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

namespace Hiruki {
	namespace Math {
		Vector2::Vector2(const Vector3 &that) : x(that.x), y(that.y) {}
		Vector2::Vector2(const Vector4 &that) : x(that.x), y(that.y) {}
	}
}
