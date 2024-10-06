#ifndef HIRUKI_GRAPHICS_TRIANGLE_H
#define HIRUKI_GRAPHICS_TRIANGLE_H

#include "graphics/texCoord.hpp"
#include "math/vector3.hpp"
#include "math/vector4.hpp"
#include <array>
#include <cstdint>

namespace Hiruki {
	namespace Graphics {
		class Triangle {
		public:
			inline Triangle(const std::array<Math::Vector4, 3> &points, const std::array<TexCoord, 3> &texCoords,
				   uint32_t color)
						: points(points), texCoords(texCoords), color(color) {};
				
			inline Triangle(const std::array<Math::Vector4, 3> &points, const std::array<TexCoord, 3> &texCoords,
				   uint32_t color, const std::array<float, 3> &vertexNormals)
						: Triangle(points, texCoords, color) {
				this->vertexNormals = vertexNormals;
			}
		
			std::array<Math::Vector4, 3> points;
			std::array<TexCoord, 3> texCoords;
			uint32_t color;
		
			float lightIntensity; // Used by Flat Shading
			std::array<float, 3> vertexNormals;
		
			// Note: Triangles should be in counterclockwise order.
			inline Math::Vector3 calculateNormal() const {
				Math::Vector3 ab = this->points[1].sub(this->points[0]).normalized();
				Math::Vector3 ac = this->points[2].sub(this->points[0]).normalized();
		
				return ab.cross(ac).normalized();
			}
		};
	}
}

#endif
