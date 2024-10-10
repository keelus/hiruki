#include "vector3.hpp"
#include "vector2.hpp"
#include "vector4.hpp"

namespace Hiruki {
	namespace Math {
		Vector3::Vector3 (const Vector2 &that) : x(that.x), y(that.y), z(0) {}
		Vector3::Vector3 (const Vector4 &that) : x(that.x), y(that.y), z(that.z) {}

		Vector3i::Vector3i (const Vector2 &that) :
			x(static_cast<int>(that.x)), y(static_cast<int>(that.y)), z(0) {}
		Vector3i::Vector3i (const Vector3 &that) :
			x(static_cast<int>(that.x)), y(static_cast<int>(that.y)), z(static_cast<int>(that.z)) {}
	}
}
