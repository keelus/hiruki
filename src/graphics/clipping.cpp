#include "clipping.hpp"
#include "graphics/texCoord.hpp"
#include "graphics/triangle.hpp"
#include <array>
#include <vector>

namespace Hiruki {
	namespace Graphics {
		Clipping::Clipping(Math::Vector2 fov, float zNear, float zFar) {
			fov = fov.div(2);
			float cosHalfFovX = cos(fov.x);
			float sinHalfFovX = sin(fov.x);
			float cosHalfFovY = cos(fov.y);
			float sinHalfFovY = sin(fov.y);
		
			m_Planes = {
				Plane(Math::Vector3::zero() ,Math::Vector3(cosHalfFovX, 0, sinHalfFovX)), // Left
				Plane(Math::Vector3::zero() ,Math::Vector3(-cosHalfFovX, 0, sinHalfFovX)), // Right
				Plane(Math::Vector3::zero() ,Math::Vector3(0, -cosHalfFovY, sinHalfFovY)), // Top
				Plane(Math::Vector3::zero() ,Math::Vector3(0, cosHalfFovY, sinHalfFovY)), // Down
				Plane(Math::Vector3(0, 0, zNear) ,Math::Vector3(0, 0, 1)), // Near
				Plane(Math::Vector3(0, 0, zFar) ,Math::Vector3(0, 0, -1)), // Far
			};
		}
		
		std::vector<Triangle> Clipping::Plane::clipTriangles(const std::vector<Triangle> &triangles) const {
			std::vector<Triangle> newTriangles;
			newTriangles.reserve(triangles.size());
			for(const Triangle &triangle : triangles) {
				std::array<PointData, 3> insidePoints, outsidePoints;
				size_t insideCount = 0, outsideCount = 0;
		
				for (int i = 0; i < 3; i++) {
					float dot = triangle.points[i].sub(m_Point).dot(m_Normal);
		
					if (dot > 0) {
						insidePoints[insideCount++] = PointData(triangle.points[i], triangle.texCoords[i], dot);
					} else {
						outsidePoints[outsideCount++] = PointData(triangle.points[i], triangle.texCoords[i], dot);
					}
				}

				static auto interpolatePointData = [&](const PointData &point1, const PointData &point2) {
					float t = point1.dot / (point1.dot - point2.dot);
					Math::Vector3 intersectionPoint = point1.position.lerp(point2.position, t);
					TexCoord texCoord = point1.texCoord.lerp(point2.texCoord, t);
					return PointData(intersectionPoint, texCoord, 0);
				};

				switch(insideCount) {
					case 0:
						continue;
					case 3:
						newTriangles.push_back(triangle);
						continue;
					case 1: {
						PointData newPoint0 = interpolatePointData(insidePoints[0], outsidePoints[0]);
						PointData newPoint1 = interpolatePointData(insidePoints[0], outsidePoints[1]);
		
						if(triangle.texture) {
							newTriangles.emplace_back(Triangle(
								{insidePoints[0].position, newPoint0.position, newPoint1.position},
								{insidePoints[0].texCoord, newPoint0.texCoord, newPoint1.texCoord},
								triangle.texture,
								triangle.lightIntensity
							));
						} else {
							newTriangles.emplace_back(Triangle(
								{insidePoints[0].position, newPoint0.position, newPoint1.position},
								triangle.color,
								triangle.lightIntensity
							));
						}

						break;
					}
					case 2: {
						PointData newPoint0 = interpolatePointData(insidePoints[0], outsidePoints[0]);
						PointData newPoint1 = interpolatePointData(insidePoints[1], outsidePoints[0]);

						if(triangle.texture) {
							newTriangles.emplace_back(Triangle(
								{insidePoints[0].position, insidePoints[1].position, newPoint0.position},
								{insidePoints[0].texCoord, insidePoints[1].texCoord, newPoint0.texCoord},
								triangle.texture,
								triangle.lightIntensity
							));
							newTriangles.emplace_back(Triangle(
								{insidePoints[1].position, newPoint1.position, newPoint0.position},
								{insidePoints[1].texCoord, newPoint1.texCoord, newPoint0.texCoord},
								triangle.texture,
								triangle.lightIntensity
							));
						} else {
							newTriangles.emplace_back(Triangle(
								{insidePoints[0].position, insidePoints[1].position, newPoint0.position},
								triangle.color,
								triangle.lightIntensity
							));
							newTriangles.emplace_back(Triangle(
								{insidePoints[1].position, newPoint1.position, newPoint0.position},
								triangle.color,
								triangle.lightIntensity
							));
						}
		
						break;
					}
				}
			}
		
			return newTriangles;
		}
		
		std::vector<Triangle> Clipping::clipTriangle(const Triangle &triangle) const {
			std::vector<Triangle> triangles = {triangle};
		
			for(const Plane &plane : m_Planes) {
				triangles = plane.clipTriangles(triangles);

				// Ensure all triangles are in Counter-Clockwise winding
				for(Triangle &triangle : triangles) {
					triangle.fixWinding();
				}
			}

			return triangles;
		}
	}
}
