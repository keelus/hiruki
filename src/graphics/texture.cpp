#include "texture.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <stdexcept>

namespace Hiruki {
	namespace Graphics {
		Texture::Texture(std::string filepath) : m_TextureSurface(nullptr), m_PixelFormat(nullptr){ // TODO: Revise this
			m_PixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

			SDL_Surface *textureSurface = IMG_Load(filepath.c_str());
				
			if(textureSurface == nullptr) {
				throw std::invalid_argument("Error loading the texture \"" + filepath + "\".\n");
			}
		
			m_TextureSurface = SDL_ConvertSurface(textureSurface, m_PixelFormat, 0);
			SDL_FreeSurface(textureSurface);
		}
		
		Texture::~Texture() {
			if(m_TextureSurface)
				SDL_FreeSurface(m_TextureSurface);
			if(m_PixelFormat)
				SDL_FreeFormat(m_PixelFormat);
		}
	}
}
