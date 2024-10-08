#include "renderPipeline.hpp"
#include "math/vector2.hpp"
#include "math/matrix4.hpp"
#include <SDL2/SDL_render.h>
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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

			Math::Matrix4 projectionMatrix = Math::Matrix4::perspective(pixelBufferHeight, pixelBufferWidth, 60.0, 0.1, 100.0);

			for(const Mesh &mesh : meshes) {
				Math::Matrix4 scaleMatrix = Math::Matrix4::scale(mesh.scale);
				Math::Matrix4 rotationMatrix = Math::Matrix4::rotateXYZ(mesh.rotation);
				Math::Matrix4 translationMatrix = Math::Matrix4::translate(mesh.translation);

				Math::Matrix4 worldMatrix = translationMatrix.mul(rotationMatrix.mul(scaleMatrix));

				for(const Mesh::Face &face: mesh.faces) {
					std::array<Math::Vector4, 3> vertices = {
						mesh.vertices[face.vertexIndex0-1],
						mesh.vertices[face.vertexIndex1-1],
						mesh.vertices[face.vertexIndex2-1],
					};
					std::array<TexCoord, 3> texCoords = {
						face.vertexTexcoord0,
						face.vertexTexcoord1,
						face.vertexTexcoord2,
					};

					std::array<Math::Vector4, 3> projectedVertices;


					for(int i = 0; i < 3; i++) {
						Math::Vector4 worldVertex = worldMatrix.mul(vertices[i]);
						Math::Vector4 projectedVertex = projectionMatrix.mul(worldVertex);
						projectedVertex = projectedVertex.perspectiveDivide();

						projectedVertex.x *= pixelBufferWidth;
						projectedVertex.y *= -pixelBufferHeight;

						projectedVertex.x += pixelBufferWidth/2.0;
						projectedVertex.y += pixelBufferHeight/2.0;

						projectedVertices[i] = projectedVertex;
					}

					auto [v0, v1, v2] = projectedVertices;
					auto [t0, t1, t2] = texCoords;
					//this->drawTriangle(v0, v1, v2);
					this->drawTriangle(v0, v1, v2, t0, t1, t2, mesh.texture);
				}
			}
		
			// Placeholder triangle
			// static Math::Vector2 v0(50, 40);
			// static Math::Vector2 v1(25, 100);
			// static Math::Vector2 v2(150, 150);

			// this->drawTriangle(v0, v1, v2);

			SDL_UpdateTexture(
				m_PixelBufferTexture,
				NULL,
				(uint32_t *)m_PixelBuffer.data(),
				(int)(pixelBufferWidth * sizeof(uint32_t))
			);
		}

		float edgeCross(const Math::Vector2 &a, const Math::Vector2 &b, const Math::Vector2 &p) {
			Math::Vector2 ab = b.sub(a);
			Math::Vector2 ap = p.sub(a);
			return ap.cross(ab);
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

		//
		//       v0
		//      /  ▲
		//     /    \
		//    /      \
		//   ▼        \
		//  v1 ------► v2
		//
		// Counter-clockwise order
		void RenderPipeline::drawTriangle(Math::Vector3 v0, Math::Vector3 v1, Math::Vector3 v2) {
			uint32_t color0 = 0xFF0000FF;
			uint32_t color1 = 0x00FF00FF;
			uint32_t color2 = 0x0000FFFF;

			float minX = static_cast<int>(std::min(v0.x, std::min(v1.x, v2.x)));
			float minY = static_cast<int>(std::min(v0.y, std::min(v1.y, v2.y)));
			float maxX = static_cast<int>(std::max(v0.x, std::max(v1.x, v2.x)));
			float maxY = static_cast<int>(std::max(v0.y, std::max(v1.y, v2.y)));

			Math::Vector2 point(minX, minY);

			//  Row edge beginning weights
			float rowW0 = edgeCross(v1, v2, point);
			float rowW1 = edgeCross(v2, v0, point);
			float rowW2 = edgeCross(v0, v1, point);

			// Column and row steps
			float colStepW0 = v2.y - v1.y;
			float colStepW1 = v0.y - v2.y;
			float colStepW2 = v1.y - v0.y;

			float rowStepW0 = v1.x - v2.x;
			float rowStepW1 = v2.x - v0.x;
			float rowStepW2 = v0.x - v1.x;

			float area = edgeCross(v0, v1, v2);

			if(area <= 0)
				return;

			// Iterate over each pixel in the bounding box of the triangle
			for(point.y = minY; point.y <= maxY; point.y++) {
				float w0 = rowW0;
				float w1 = rowW1;
				float w2 = rowW2;

				for(point.x = minX; point.x <= maxX; point.x++) {
					if(w0 >= 0 && w1 >= 0 && w2 >= 0) {
						//std::cout << "Hi!" << std::endl;
						//float alpha = wE01 / area;
						//float beta = wE12 / area;
						//float gamma = 1 - alpha - beta;
						float alpha = w0 / area;
						float beta = w1 / area;
						float gamma = w2 / area;

						uint32_t finalColor =
								colorPercent(color0, alpha) +
								colorPercent(color1, beta) +
								colorPercent(color2, gamma);

						drawPixel(point.x, point.y, finalColor);
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

		void RenderPipeline::drawTriangle(
				Math::Vector4 v0, Math::Vector4 v1, Math::Vector4 v2,
				TexCoord t0, TexCoord t1, TexCoord t2,
				std::shared_ptr<Texture> texture
		) {
			uint32_t color0 = 0xFF0000FF;
			uint32_t color1 = 0x00FF00FF;
			uint32_t color2 = 0x0000FFFF;

			float minX = static_cast<int>(std::min(v0.x, std::min(v1.x, v2.x)));
			float minY = static_cast<int>(std::min(v0.y, std::min(v1.y, v2.y)));
			float maxX = static_cast<int>(std::max(v0.x, std::max(v1.x, v2.x)));
			float maxY = static_cast<int>(std::max(v0.y, std::max(v1.y, v2.y)));

			Math::Vector2 point(minX, minY);

			//  Row edge beginning weights
			float rowW0 = edgeCross(v1, v2, point);
			float rowW1 = edgeCross(v2, v0, point);
			float rowW2 = edgeCross(v0, v1, point);

			// Column and row steps
			float colStepW0 = v2.y - v1.y;
			float colStepW1 = v0.y - v2.y;
			float colStepW2 = v1.y - v0.y;

			float rowStepW0 = v1.x - v2.x;
			float rowStepW1 = v2.x - v0.x;
			float rowStepW2 = v0.x - v1.x;

			int area = edgeCross(v0, v1, v2);

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
						
						float texU0 = t0.u * wRecip0;
						float texU1 = t1.u * wRecip1;
						float texU2 = t2.u * wRecip2;

						float texV0 = t0.v * wRecip0;
						float texV1 = t1.v * wRecip1;
						float texV2 = t2.v * wRecip2;

						float uInterpolated = texU0 * alpha + texU1 * beta + texU2 * gamma;
						float vInterpolated = texV0 * alpha + texV1 * beta + texV2 * gamma;
						float wInterpolated = wRecip0 * alpha + wRecip1 * beta + wRecip2 * gamma;

						uInterpolated /= wInterpolated;
						vInterpolated /= wInterpolated;

						uint32_t finalColor = texture->pickColor(uInterpolated, vInterpolated);

						int index = point.y * pixelBufferWidth + point.x;
						wInterpolated = 1 - wInterpolated;
						if (index >= 0 && index < pixelBufferWidth * pixelBufferHeight) {
							if (wInterpolated < m_DepthBuffer[index]) {
								drawPixel(point.x, point.y, finalColor);
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

		inline void RenderPipeline::drawPixel(int x, int y, uint32_t color) {
			if(x >= 0 && x < pixelBufferWidth && y >= 0 && y < pixelBufferHeight)
				m_PixelBuffer[y * pixelBufferWidth + x] = color;
		}
	}
}
