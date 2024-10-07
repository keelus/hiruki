#ifndef HIRUKI_GRAPHICS_RENDER_PIPELINE_H
#define HIRUKI_GRAPHICS_RENDER_PIPELINE_H

#include "graphics/mesh.hpp"
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <vector>

namespace Hiruki {
	namespace Graphics {
		class RenderPipeline {
			public:
				RenderPipeline() : pixelBufferWidth(0), pixelBufferHeight(0) {}
				RenderPipeline(int windowWidth, int windowHeight, SDL_Renderer *renderer);

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

			private:
				SDL_Texture* m_PixelBufferTexture;
				std::vector<uint32_t> m_PixelBuffer;
		};
	}
}

#endif
