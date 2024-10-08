#ifndef HIRUKI_GRAPHICS_RENDER_PIPELINE_H
#define HIRUKI_GRAPHICS_RENDER_PIPELINE_H

#include "graphics/mesh.hpp"
#include "graphics/texture.hpp"
#include "math/vector3.hpp"
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <memory>
#include <vector>

namespace Hiruki {
	namespace Graphics {
		class RenderPipeline {
			public:
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
						pixelBufferWidth = other.pixelBufferWidth;
						pixelBufferHeight = other.pixelBufferHeight;
						other.m_PixelBufferTexture = nullptr;
					}
					return *this;
				}

				void drawTriangle(Math::Vector3 v0, Math::Vector3 v1, Math::Vector3 v2);
				void drawTriangle(
						Math::Vector3 v0, Math::Vector3 v1, Math::Vector3 v2,
						TexCoord t0, TexCoord t1, TexCoord t2,
						std::shared_ptr<Texture> texture
				);

				void drawPixel(int x, int y, uint32_t color);

			private:
				SDL_Texture* m_PixelBufferTexture;
				std::vector<uint32_t> m_PixelBuffer;
		};
	}
}

#endif
