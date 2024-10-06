#include "vector3.hpp"
#include "vector2.hpp"
#include "vector4.hpp"

namespace Hiruki {
	namespace Math {
		Vector3::Vector3 (const Vector2 &that) : x(that.x), y(that.y), z(0) {}
		Vector3::Vector3 (const Vector4 &that) : x(that.x), y(that.y), z(that.z) {}
	}
}
