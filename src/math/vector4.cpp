#include "vector3.hpp"
#include "vector2.hpp"
#include "vector4.hpp"

namespace Hiruki {
	namespace Math {
		Vector4::Vector4 (const Vector2 &that) : x(that.x), y(that.y), z(0), w(1) {}
		Vector4::Vector4 (const Vector3 &that) : x(that.x), y(that.y), z(that.z), w(1) {}
	}
}
