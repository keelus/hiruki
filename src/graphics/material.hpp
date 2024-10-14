#ifndef HIRUKI_GRAPHICS_MATERIAL_H
#define HIRUKI_GRAPHICS_MATERIAL_H

#include "graphics/texture.hpp"
#include <string>

namespace Hiruki {
	namespace Graphics {
		class Material {
		public:
			Material() : m_Identifier(""), m_Texture() {};
			Material(const std::string &identifier, const std::string &texturePath) : m_Identifier(identifier), m_Texture(texturePath) {};

			~Material() {}

			inline const Graphics::Texture &getTexture() const {
				return m_Texture;
			}

		private:
			std::string m_Identifier;
			Graphics::Texture m_Texture;
		};
	}
}

#endif
