#include "renderPipeline.hpp"
#include "graphics/clipping.hpp"
#include "math/vector2.hpp"
#include "math/matrix4.hpp"
#include "math/vector3.hpp"
#include <SDL2/SDL_render.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace Hiruki {
	namespace Graphics {
		RenderPipeline::RenderPipeline(int renderWidth, int renderHeight, SDL_Renderer *renderer)
			: pixelBufferWidth(renderWidth), pixelBufferHeight(renderHeight) {
			m_PixelBufferTexture = SDL_CreateTexture(
										renderer,
										SDL_PIXELFORMAT_RGBA8888,
										SDL_TEXTUREACCESS_STREAMING, 
										renderWidth, renderHeight
			);
			m_PixelBuffer.resize(renderWidth * renderHeight, 0);
			m_DepthBuffer.resize(renderWidth * renderHeight, 0);
			m_DrawMode = DrawMode::SOLID;
			m_ShadingMode = ShadingMode::FLAT;
		}
		
		RenderPipeline::~RenderPipeline() {
			if(m_PixelBufferTexture)
				SDL_DestroyTexture(m_PixelBufferTexture);
		}

		void RenderPipeline::render(const std::vector<Mesh> &meshes) {
			std::memset(m_PixelBuffer.data(), 0, m_PixelBuffer.size() * sizeof(uint32_t));
			for(int i = 0; i < m_DepthBuffer.size(); i++) {
				m_DepthBuffer[i] = 1.0f;
			}

			static const float FOV_Y = 60.0;
			static const float Z_FAR = 50.0;
			static const float Z_NEAR = 0.1;

			float fovy = FOV_Y * M_PI / 180.0;
			float aspectX = static_cast<float>(pixelBufferWidth) / pixelBufferHeight;
			float fovx = atan(tan((FOV_Y * M_PI / 180.0) / 2.0) * aspectX) * 2;


			Math::Matrix4 projectionMatrix = Math::Matrix4::perspective(pixelBufferHeight, pixelBufferWidth, FOV_Y, Z_NEAR, Z_FAR);

			Clipping clipper(Math::Vector2(fovx, fovy), Z_NEAR, Z_FAR);

			for(const Mesh &mesh : meshes) {
				Math::Matrix4 scaleMatrix = Math::Matrix4::scale(mesh.scale);
				Math::Matrix4 rotationMatrix = Math::Matrix4::rotateXYZ(mesh.rotation);
				Math::Matrix4 translationMatrix = Math::Matrix4::translate(mesh.translation);

				Math::Matrix4 worldMatrix = translationMatrix.mul(rotationMatrix.mul(scaleMatrix));

				std::vector<Math::Vector3> vertexNormals(mesh.vertices.size(), Math::Vector3::zero());
				std::vector<float> vertexLightIntensity(mesh.vertices.size(), 0.0f);

				for(const Mesh::Face &face: mesh.faces) {
					Triangle triangle(
						{
							mesh.vertices[face.vertexIndices.x-1],
							mesh.vertices[face.vertexIndices.y-1],
							mesh.vertices[face.vertexIndices.z-1]
						}, 
						{face.texCoords[0], face.texCoords[1], face.texCoords[2]},
						mesh.texture,
						{}
					);


					for(int i = 0; i < 3; i++) {
						triangle.points[i] = worldMatrix.mul(triangle.points[i]);
					}

					Math::Vector3 faceNormal = triangle.calculateNormal();

					vertexNormals[face.vertexIndices.x-1] = vertexNormals[face.vertexIndices.x-1].add(faceNormal);
					vertexNormals[face.vertexIndices.y-1] = vertexNormals[face.vertexIndices.y-1].add(faceNormal);
					vertexNormals[face.vertexIndices.z-1] = vertexNormals[face.vertexIndices.z-1].add(faceNormal);
				}

				// Goraud shading
				if(m_ShadingMode == ShadingMode::GORAUD) {
					// Directional light
					Math::Vector3 lightDirection(-1, 0, 0);
					for(int i = 0; i < vertexNormals.size(); i++){
						Math::Vector3 normal = vertexNormals[i].normalized();
						float dot = lightDirection.dot(normal);
						// Convert from [-1, 1] to [0, 1] light intensity
						dot = (1 - dot) / 2.0f;
						vertexLightIntensity[i] = dot;
					}
				}
				for(const Mesh::Face &face: mesh.faces) {
					Triangle triangle(
						{
							mesh.vertices[face.vertexIndices.x-1],
							mesh.vertices[face.vertexIndices.y-1],
							mesh.vertices[face.vertexIndices.z-1]
						}, 
						{face.texCoords[0], face.texCoords[1], face.texCoords[2]},
						mesh.texture,
						{}
					);

					for(int i = 0; i < 3; i++) {
						triangle.points[i] = worldMatrix.mul(triangle.points[i]);
					}

					// Cull triangles
					bool shouldDrawTriangle = true;
					{
						Math::Vector3 triangleNormal = triangle.calculateNormal();
						Math::Vector3 cameraPosition = Math::Vector3::zero();
						Math::Vector3 cameraRay = cameraPosition.sub(triangle.points[0]);
						float dot = cameraRay.dot(triangleNormal);
						shouldDrawTriangle = dot >= 0.0;
					}

					if(!shouldDrawTriangle)
						continue;
			
					switch(m_ShadingMode) {
						case ShadingMode::NONE:
							triangle.vertexLights[0] = 1.0f;
							triangle.vertexLights[1] = 1.0f;
							triangle.vertexLights[2] = 1.0f;
							break;
						case ShadingMode::FLAT: {
							// Directional light
							Math::Vector3 lightDirection(0, 0, 1);
							float dot = lightDirection.dot(triangle.calculateNormal());
							// Convert from [-1, 1] to [0, 1] light intensity
							dot = (1 - dot) / 2.0f;
							triangle.vertexLights[0] = dot;
							triangle.vertexLights[1] = dot;
							triangle.vertexLights[2] = dot;
							break;
						}
						case ShadingMode::GORAUD:
							triangle.vertexLights[0] = vertexLightIntensity[face.vertexIndices.x-1];
							triangle.vertexLights[1] = vertexLightIntensity[face.vertexIndices.y-1];
							triangle.vertexLights[2] = vertexLightIntensity[face.vertexIndices.z-1];
							break;
					}

					std::vector<Triangle> trianglesAfterClipping = clipper.clipTriangle(triangle);
					for(Triangle &clippedTriangle: trianglesAfterClipping) {
						for(int i = 0; i < 3; i++) {
							Math::Vector4 projectedVertex = projectionMatrix.mul(clippedTriangle.points[i]);
							projectedVertex = projectedVertex.perspectiveDivide();

							projectedVertex.y *= -1;

							projectedVertex.x *= pixelBufferWidth/2.0;
							projectedVertex.y *= pixelBufferHeight/2.0;

							projectedVertex.x += pixelBufferWidth/2.0;
							projectedVertex.y += pixelBufferHeight/2.0;

							clippedTriangle.points[i] = projectedVertex;
						}

						static bool wireframeEnabled = false; // Temporal
						float area = clippedTriangle.calculateArea2D();
						if(area > 0) {
							this->drawTriangle(clippedTriangle);
							if(wireframeEnabled) {
								this->drawTriangleWireframe(clippedTriangle, 0xFF0000FF);
							}
						}
					}
				}
			}

			SDL_UpdateTexture(
				m_PixelBufferTexture,
				NULL,
				(uint32_t *)m_PixelBuffer.data(),
				(int)(pixelBufferWidth * sizeof(uint32_t))
			);
		}

		uint32_t colorPercent(uint32_t color, float percent) {
			uint8_t red = (color >> 24) & 0xFF;
			uint8_t green = (color >> 16) & 0xFF;
			uint8_t blue = (color >> 8) & 0xFF;
			uint8_t alpha = color & 0xFF;

			red = static_cast<uint8_t>(red * percent);
			green = static_cast<uint8_t>(green * percent);
			blue = static_cast<uint8_t>(blue * percent);
			alpha = static_cast<uint8_t>(alpha * percent);

			return (red << 24) | (green << 16) | (blue << 8) | alpha;
		}

		//       v0
		//      /  ▲
		//     /    \
		//    /      \
		//   ▼        \
		//  v1 ------► v2
		//
		// Draw the triangle in counter-clockwise order
		void RenderPipeline::drawTriangle(const Triangle &triangle) {
			const Math::Vector4 &v0 = triangle.points[0];
			const Math::Vector4 &v1 = triangle.points[1];
			const Math::Vector4 &v2 = triangle.points[2];

			const TexCoord &t0 = triangle.texCoords[0];
			const TexCoord &t1 = triangle.texCoords[1];
			const TexCoord &t2 = triangle.texCoords[2];

			float minX = static_cast<int>(std::min(v0.x, std::min(v1.x, v2.x)));
			float minY = static_cast<int>(std::min(v0.y, std::min(v1.y, v2.y)));
			float maxX = static_cast<int>(std::max(v0.x, std::max(v1.x, v2.x)));
			float maxY = static_cast<int>(std::max(v0.y, std::max(v1.y, v2.y)));

			Math::Vector2 point(minX, minY);

			//  Row edge beginning weights
			float rowW0 = Math::Vector2::edgeCross(v1, v2, point);
			float rowW1 = Math::Vector2::edgeCross(v2, v0, point);
			float rowW2 = Math::Vector2::edgeCross(v0, v1, point);

			// Column and row steps
			float colStepW0 = v2.y - v1.y;
			float colStepW1 = v0.y - v2.y;
			float colStepW2 = v1.y - v0.y;

			float rowStepW0 = v1.x - v2.x;
			float rowStepW1 = v2.x - v0.x;
			float rowStepW2 = v0.x - v1.x;

			int area = Math::Vector2::edgeCross(v0, v1, v2);

			if(area <= 0)
				return;

			// Iterate over each pixel in the bounding box of the triangle
			for(point.y = minY; point.y <= maxY; point.y++) {
				float w0 = rowW0;
				float w1 = rowW1;
				float w2 = rowW2;

				for(point.x = minX; point.x <= maxX; point.x++) {
					if(w0 >= 0 && w1 >= 0 && w2 >= 0) {
						float alpha = w0 / area;
						float beta = w1 / area;
						float gamma = w2 / area;

						float wRecip0 = 1 / v0.w;
						float wRecip1 = 1 / v1.w;
						float wRecip2 = 1 / v2.w;

						float wInterpolated = wRecip0 * alpha + wRecip1 * beta + wRecip2 * gamma;

						float lightIntensity = triangle.vertexLights[0] * alpha +
											triangle.vertexLights[1] * beta +
											triangle.vertexLights[2] * gamma;

						lightIntensity = std::max(0.0f, std::min(1.0f, lightIntensity));

						uint32_t finalColor = 0;
						switch(m_DrawMode) {
							case DrawMode::SOLID:
								finalColor = triangle.color;
								break;
							case DrawMode::GRADIENT:
								finalColor = 
									colorPercent(0xFF0000FF, alpha) +
									colorPercent(0x00FF00FF, beta) +
									colorPercent(0x0000FF00, gamma);
								break;
							case DrawMode::TEXTURED:
								if(!triangle.texture) {
									throw std::invalid_argument("The triangle to draw has no texture attached to it.");
								}

								float texU0 = t0.u * wRecip0;
								float texU1 = t1.u * wRecip1;
								float texU2 = t2.u * wRecip2;

								float texV0 = t0.v * wRecip0;
								float texV1 = t1.v * wRecip1;
								float texV2 = t2.v * wRecip2;

								float uInterpolated = texU0 * alpha + texU1 * beta + texU2 * gamma;
								float vInterpolated = texV0 * alpha + texV1 * beta + texV2 * gamma;

								uInterpolated /= wInterpolated;
								vInterpolated /= wInterpolated;

								finalColor = triangle.texture->pickColor(uInterpolated, vInterpolated);
								break;
						}

						int index = point.y * pixelBufferWidth + point.x;
						wInterpolated = 1 - wInterpolated;
						if (index >= 0 && index < pixelBufferWidth * pixelBufferHeight) {
							if (wInterpolated < m_DepthBuffer[index]) {
								drawPixel(point.x, point.y, colorPercent(finalColor, lightIntensity));
								m_DepthBuffer[index] = wInterpolated;
							}
						}
					}
					
					w0 += colStepW0;
					w1 += colStepW1;
					w2 += colStepW2;
				}

				rowW0 += rowStepW0;
				rowW1 += rowStepW1;
				rowW2 += rowStepW2;
			}
		}

		void RenderPipeline::drawTriangleWireframe(const Triangle &triangle, uint32_t color) {
			drawLine(triangle.points[0], triangle.points[1], color);
			drawLine(triangle.points[1], triangle.points[2], color);
			drawLine(triangle.points[2], triangle.points[0], color);
		}

		void RenderPipeline::drawLine(Math::Vector4 v0, Math::Vector4 v1, uint32_t color) {
			if (v0.x == v1.x && v0.y == v1.y)
				return;

			float w0Recip = 1 / v0.w;
			float w1Recip = 1 / v1.w;

			Math::Vector2 sideLengths = v1.sub(v0);
			float numSteps = std::max(std::fabs(sideLengths.x), std::fabs(sideLengths.y));
			Math::Vector2 stepSize = sideLengths.div(static_cast<float>(numSteps));
	
			Math::Vector2 point(v0);
			for (int i = 0; i <= numSteps; i++) {
				float tx = (point.x - v0.x) / (v1.x - v0.x);
				float ty = (point.y - v0.y) / (v1.y - v0.y);
				float t = (tx + ty)/2;

				float w = w0Recip + t * (w1Recip - w0Recip);
				w = 1 - w - 0.01;

				int index = std::roundf(point.y) * pixelBufferWidth + std::roundf(point.x);
				if (index >= 0 && index < pixelBufferWidth * pixelBufferHeight) {
					if (w< m_DepthBuffer[index]) {
						drawPixel(std::roundf(point.x), std::roundf(point.y), color);
						m_DepthBuffer[index] = w;
					}
				}
				
				point = point.add(stepSize);
			}
		}

		inline void RenderPipeline::drawPixel(int x, int y, uint32_t color) {
			if(x >= 0 && x < pixelBufferWidth && y >= 0 && y < pixelBufferHeight)
				m_PixelBuffer[y * pixelBufferWidth + x] = color;
		}
	}
}
