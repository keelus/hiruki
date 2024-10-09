#ifndef HIRUKI_GRAPHICS_CLIPPING_H
#define HIRUKI_GRAPHICS_CLIPPING_H

#include "graphics/triangle.hpp"
#include "math/vector3.hpp"
#include <array>
#include <vector>

namespace Hiruki {
	namespace Graphics {
		class Clipping {
			public:
				class Plane {
				public:
					Plane() {};
					Plane(Math::Vector3 point, Math::Vector3 normal) : m_Point(point), m_Normal(normal) {};
		
					std::vector<Triangle> clipTriangles(const std::vector<Triangle> &triangle) const;
		
					Math::Vector3 m_Point;
					Math::Vector3 m_Normal;
				private:
			};
		
			Clipping(Math::Vector2 fov, float zNear, float zFar);
			std::vector<Triangle> clipTriangle(const Triangle &triangle) const;
			private:
				std::array<Plane, 6> m_Planes;
		};
	}
}

#endif
