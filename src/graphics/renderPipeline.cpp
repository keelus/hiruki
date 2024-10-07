#include "renderPipeline.hpp"
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

namespace Hiruki {
	namespace Graphics {
		RenderPipeline::RenderPipeline(int windowWidth, int windowHeight, SDL_Renderer *renderer)
			: pixelBufferWidth(windowWidth), pixelBufferHeight(windowHeight) {
			m_PixelBufferTexture = SDL_CreateTexture(
										renderer,
										SDL_PIXELFORMAT_RGBA8888,
										SDL_TEXTUREACCESS_STREAMING, 
					  					windowWidth, windowHeight
			);
			m_PixelBuffer.resize(windowWidth * windowHeight, 0);
		}
		
		RenderPipeline::~RenderPipeline() {
			if(m_PixelBufferTexture)
				SDL_DestroyTexture(m_PixelBufferTexture);
		}

		void RenderPipeline::render(const std::vector<Mesh> &meshes) {
			// Noise
			for(int i = 0; i < pixelBufferWidth * pixelBufferHeight; i++) {
				m_PixelBuffer[i] = (uint32_t)rand();
			}

			SDL_UpdateTexture(
				m_PixelBufferTexture,
				NULL,
				(uint32_t *)m_PixelBuffer.data(),
				(int)(pixelBufferWidth * sizeof(uint32_t))
			);
		}
	}
}
