#ifndef HIRUKI_GRAPHICS_TEXTURE_H
#define HIRUKI_GRAPHICS_TEXTURE_H

#include "SDL_pixels.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <cmath>
#include <string>

namespace Hiruki {
	namespace Graphics {
		class Texture {
			public:
				Texture() : m_TextureSurface(nullptr), m_PixelFormat(nullptr) {}

				Texture(std::string filepath);
				~Texture();
		
				inline uint32_t pickColor(float u, float v) const {
					u = std::fmod(u, 1.0f);
					if(u < 0.0) u += 1.0f;

					v = std::fmod(v, 1.0f);
					if(v < 0.0) v += 1.0f;

					int x = u * (m_TextureSurface->w - 1);
					int y = (1 - v) * (m_TextureSurface->h - 1);
		
					x %= m_TextureSurface->w;
					y %= m_TextureSurface->h;

					int index = y * m_TextureSurface->w + x;
					if(index >= 0 && index < m_TextureSurface->w * m_TextureSurface->h) {
						return ((uint32_t *)m_TextureSurface->pixels)[index];
					}
					return 0xFF00DCFF;
				}
		
			private:
				SDL_Surface *m_TextureSurface;
				SDL_PixelFormat *m_PixelFormat;
		};
	}
}

#endif 
