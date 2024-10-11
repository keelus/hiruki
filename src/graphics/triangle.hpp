#ifndef HIRUKI_GRAPHICS_TRIANGLE_H
#define HIRUKI_GRAPHICS_TRIANGLE_H

#include "graphics/texCoord.hpp"
#include "graphics/texture.hpp"
#include "math/vector3.hpp"
#include "math/vector4.hpp"
#include <array>
#include <cstdint>
#include <memory>

namespace Hiruki {
	namespace Graphics {
		class Triangle {
		public:
			Triangle() : texture(nullptr), color(0) {};
			
			inline Triangle(const std::array<Math::Vector4, 3> &points, const std::array<TexCoord, 3> &texCoords,
				   std::shared_ptr<Texture> texture, std::array<float, 3> vertexLights)
						: points(points), texCoords(texCoords), texture(texture), color(0xFFFFFFFF),
						vertexLights(vertexLights) {};
				
			inline Triangle(const std::array<Math::Vector4, 3> &points, uint32_t color, std::array<float, 3> vertexLights)
						: points(points), color(color), texture(nullptr), vertexLights(vertexLights) {}
		
			// Note: Triangles should be in counterclockwise order.
			inline Math::Vector3 calculateNormal() const {
				Math::Vector3 ab = this->points[1].sub(this->points[0]).normalized();
				Math::Vector3 ac = this->points[2].sub(this->points[0]).normalized();
		
				return ac.cross(ab).normalized();
			}

			inline float calculateArea2D() const {
				return Math::Vector2::edgeCross(this->points[0], this->points[1], this->points[2]);
			}

			// Forces a counter-clockwise winding order, useful while clipping
			inline void fixWinding() {
				Math::Vector3 triangleNormal = this->calculateNormal();
				Math::Vector3 cameraRay = Math::Vector3::zero().sub(this->points[0]);
				if(cameraRay.dot(triangleNormal) < 0.0){
					std::swap(this->points[1], this->points[2]);
					std::swap(this->texCoords[1], this->texCoords[2]);
					std::swap(this->vertexLights[1], this->vertexLights[2]);
				}
			}

			const std::shared_ptr<Texture> texture;
			const uint32_t color;

			std::array<Math::Vector4, 3> points;
			std::array<TexCoord, 3> texCoords;
		
			std::array<float, 3> vertexLights;
		};
	}
}

#endif
