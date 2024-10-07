#include "renderPipeline.hpp"
#include "math/vector2.hpp"
#include <SDL2/SDL_render.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
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
		}
		
		RenderPipeline::~RenderPipeline() {
			if(m_PixelBufferTexture)
				SDL_DestroyTexture(m_PixelBufferTexture);
		}


		void RenderPipeline::render(const std::vector<Mesh> &meshes) {
			std::memset(m_PixelBuffer.data(), 0, m_PixelBuffer.size() * sizeof(uint32_t));
		
			// Placeholder triangle
			static Math::Vector2 v0(50, 40);
			static Math::Vector2 v1(25, 100);
			static Math::Vector2 v2(150, 150);

			this->drawTriangle(v0, v1, v2);

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

			int minX = static_cast<int>(std::min(v0.x, std::min(v1.x, v2.x)));
			int minY = static_cast<int>(std::min(v0.y, std::min(v1.y, v2.y)));
			int maxX = static_cast<int>(std::max(v0.x, std::max(v1.x, v2.x)));
			int maxY = static_cast<int>(std::max(v0.y, std::max(v1.y, v2.y)));

			Math::Vector2 point(minX, minY);

			//  Row edge beginning weights
			int rowE01 = edgeCross(v0, v1, point);
			int rowE12 = edgeCross(v1, v2, point);
			int rowE20 = edgeCross(v2, v0, point);

			// Column and row steps
			int colStepE01 = v1.y - v0.y;
			int colStepE12 = v2.y - v1.y;
			int colStepE20 = v0.y - v2.y;

			int rowStepE01 = v0.x - v1.x;
			int rowStepE12 = v1.x - v2.x;
			int rowStepE20 = v2.x - v0.x;

			int area = edgeCross(v0, v1, v2);

			if(area <= 0)
				return;

			// Iterate over each pixel in the bounding box of the triangle
			for(point.y = minY; point.y <= maxY; point.y++) {
				int wE01 = rowE01;
				int wE12 = rowE12;
				int wE20 = rowE20;

				for(point.x = minX; point.x <= maxX; point.x++) {
					if((wE01 | wE12 | wE20) >= 0) {
						float alpha = static_cast<float>(wE01) / area;
						float beta = static_cast<float>(wE12) / area;
						float gamma = 1 - alpha - beta;

						uint32_t finalColor =
								colorPercent(color0, alpha) +
								colorPercent(color1, beta) +
								colorPercent(color2, gamma);

						drawPixel(point.x, point.y, finalColor);
					}
					
					wE01 += colStepE01;
					wE12 += colStepE12;
					wE20 += colStepE20;
				}

				rowE01 += rowStepE01;
				rowE12 += rowStepE12;
				rowE20 += rowStepE20;
			}
		}

		inline void RenderPipeline::drawPixel(int x, int y, uint32_t color) {
			m_PixelBuffer[y * pixelBufferWidth + x] = color;
		}
	}
}
