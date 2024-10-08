#ifndef HIRUKI_GRAPHICS_TEXTURE_H
#define HIRUKI_GRAPHICS_TEXTURE_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <string>

namespace Hiruki {
	namespace Graphics {
		class Texture {
			public:
				Texture() {
					m_TextureSurface = nullptr;
					m_PixelFormat = nullptr;
				}

				Texture(std::string filepath);
				~Texture();
		
				inline uint32_t pickColor(float u, float v) {
					int x = u * (m_TextureSurface->w - 1);
					int y = (1 - v) * (m_TextureSurface->w - 1);
		
					x %= m_TextureSurface->w;
					y %= m_TextureSurface->h;
		
					return ((uint32_t *)m_TextureSurface->pixels)[y * m_TextureSurface->w + x];
				}
		
		
			private:
				SDL_Surface *m_TextureSurface;
				SDL_PixelFormat *m_PixelFormat;
		};
	}
}

#endif 
