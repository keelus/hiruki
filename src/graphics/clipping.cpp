#include "clipping.hpp"
#include "graphics/triangle.hpp"
#include <array>
#include <cstdio>
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
				float dots[3];
				for (int i = 0; i < 3; i++) {
					Math::Vector3 p = triangle.points[i];
					Math::Vector3 subtraction = p.sub(m_Point);
					float dotResult = subtraction.dot(m_Normal);
					dots[i] = dotResult;
				}
		
				std::array<Math::Vector3, 3> insidePoints;
				std::array<TexCoord, 3> insideTexCoords;
				std::array<float, 3> insideDots;
				std::array<Math::Vector3, 3> outsidePoints;
				std::array<TexCoord, 3> outsideTexCoords;
				std::array<float, 3> outsideDots;

				size_t insideCount = 0;
				size_t outsideCount = 0;

				for (int i = 0; i < 3; i++) {
					float dot = dots[i];
		
					if (dot > 0) {
						insidePoints[insideCount] = (triangle.points[i]);
						insideDots[insideCount]=(dot);
						insideTexCoords[insideCount]=(triangle.texCoords[i]);
						insideCount++;
					} else {
						outsidePoints[outsideCount]=(triangle.points[i]);
						outsideDots[outsideCount]=(dot);
						outsideTexCoords[outsideCount]=(triangle.texCoords[i]);
						outsideCount++;
					}
				}
		
				if (insideCount == 0) {
					continue;
				} else if (insideCount == 3) {
					newTriangles.push_back(triangle);
				} else if (insideCount == 1) {
					float insideDot = insideDots[0];
		
					// Find the interpolation factor t
					float t0 = insideDot / (insideDot - outsideDots[0]);
					float t1 = insideDot / (insideDot - outsideDots[1]);

					//  Calculate the intersection point I = Q1 + t(Q2-Q1)
					Math::Vector3 intersectionPoint0(
						lerp(insidePoints[0].x, outsidePoints[0].x, t0),
						lerp(insidePoints[0].y, outsidePoints[0].y, t0),
						lerp(insidePoints[0].z, outsidePoints[0].z, t0)
					);
					Math::Vector3 intersectionPoint1(
						lerp(insidePoints[0].x, outsidePoints[1].x, t1),
						lerp(insidePoints[0].y, outsidePoints[1].y, t1),
						lerp(insidePoints[0].z, outsidePoints[1].z, t1)
					);
		
					TexCoord texcoord0(
						lerp(insideTexCoords[0].u, outsideTexCoords[0].u, t0),
						lerp(insideTexCoords[0].v, outsideTexCoords[0].v, t0)
					);
					TexCoord texcoord1(
						lerp(insideTexCoords[0].u, outsideTexCoords[1].u, t1),
						lerp(insideTexCoords[0].v, outsideTexCoords[1].v, t1)
					);
		
					// TODO: Color!
					Triangle newTriangle(
						{insidePoints[0], intersectionPoint0, intersectionPoint1},
						{insideTexCoords[0], texcoord0, texcoord1}, triangle.texture
					);
		
					newTriangles.push_back(newTriangle);
				} else {
					float insideDot0 = insideDots[0];
					float insideDot1 = insideDots[1];
					Math::Vector3 insidePoint0 = insidePoints[0];
					Math::Vector3 insidePoint1 = insidePoints[1];
		
					float outsideDot = outsideDots[0];
					Math::Vector3 outsidePoint = outsidePoints[0];
		
					float t0 = insideDot0 / (insideDot0 - outsideDot);
					float t1 = insideDot1 / (insideDot1 - outsideDot);
		
					Math::Vector3 intersectionPoint0(
						 lerp(insidePoint0.x, outsidePoint.x, t0),
						 lerp(insidePoint0.y, outsidePoint.y, t0),
						 lerp(insidePoint0.z, outsidePoint.z, t0)
					);
					Math::Vector3 intersectionPoint1(
						 lerp(insidePoint1.x, outsidePoint.x, t1),
						 lerp(insidePoint1.y, outsidePoint.y, t1),
						 lerp(insidePoint1.z, outsidePoint.z, t1)
					);
		
					TexCoord texcoord0(
						lerp(insideTexCoords[0].u, outsideTexCoords[0].u, t0),
						lerp(insideTexCoords[0].v, outsideTexCoords[0].v, t0)
					);
					TexCoord texcoord1(
						lerp(insideTexCoords[1].u, outsideTexCoords[0].u, t1),
						lerp(insideTexCoords[1].v, outsideTexCoords[0].v, t1)
					);
		
					Triangle triangle0(
						{insidePoint0, insidePoint1, intersectionPoint0},
						{insideTexCoords[0], insideTexCoords[1], texcoord0},
						triangle.texture
					);
					Triangle triangle1(
						{insidePoint1, intersectionPoint1, intersectionPoint0},
						{insideTexCoords[1], texcoord1, texcoord0},
						triangle.texture
					);
		
		
					newTriangles.push_back(triangle0);
					newTriangles.push_back(triangle1);
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
