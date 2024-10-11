#ifndef HIRUKI_GRAPHICS_RENDER_PIPELINE_H
#define HIRUKI_GRAPHICS_RENDER_PIPELINE_H

#include "graphics/mesh.hpp"
#include "graphics/triangle.hpp"
#include "math/vector3.hpp"
#include <SDL2/SDL_render.h>
#include <vector>

namespace Hiruki {
	namespace Graphics {
		class RenderPipeline {
			public:
				enum class DrawMode {
					SOLID,
					GRADIENT, // RGB interpolated triangle
					TEXTURED,
				};
				enum class ShadingMode {
					NONE,
					FLAT,
					GORAUD
				};
				RenderPipeline() : pixelBufferWidth(0), pixelBufferHeight(0) {}
				RenderPipeline(int renderWidth, int renderHeight, SDL_Renderer *renderer);

				~RenderPipeline();
				
				void render(const std::vector<Mesh> &meshes);

				SDL_Texture * pixelBufferTexture() {
					return m_PixelBufferTexture;
				}

				int pixelBufferWidth;
				int pixelBufferHeight;

				RenderPipeline& operator=(RenderPipeline&& other) noexcept {
					if (this != &other) {
						m_PixelBufferTexture = other.m_PixelBufferTexture;
						m_PixelBuffer = std::move(other.m_PixelBuffer);
						m_DepthBuffer = std::move(other.m_DepthBuffer);
						pixelBufferWidth = other.pixelBufferWidth;
						pixelBufferHeight = other.pixelBufferHeight;
						other.m_PixelBufferTexture = nullptr;
						m_DrawMode = other.m_DrawMode;
						m_ShadingMode = other.m_ShadingMode;
					}
					return *this;
				}
				
				void drawTriangle(const Triangle &triangle);

				void drawTriangleWireframe(const Triangle &triangle, uint32_t color);
				void drawLine(Math::Vector4 v0, Math::Vector4 v1, uint32_t color);

				void drawPixel(int x, int y, uint32_t color);

			private:
				SDL_Texture* m_PixelBufferTexture;
				std::vector<uint32_t> m_PixelBuffer;
				std::vector<float> m_DepthBuffer;

				DrawMode m_DrawMode;
				ShadingMode m_ShadingMode;
		};
	}
}

#endif
