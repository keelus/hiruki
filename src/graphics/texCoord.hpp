#ifndef HIRUKI_GRAPHICS_TEXCOORD_H
#define HIRUKI_GRAPHICS_TEXCOORD_H

#include "../math/vector2.hpp"

namespace Hiruki {
	namespace Graphics {
		class TexCoord {
		public:
			constexpr inline TexCoord() : u(0), v(0) {}
			inline TexCoord(float u, float v) : u(u), v(v) {}
		
			TexCoord(const Math::Vector2 &vector);

			inline TexCoord lerp(const TexCoord &that, float t) const {
				return TexCoord (
					std::lerp(this->u, that.u, t),
					std::lerp(this->v, that.v, t)
				);
			}
		
			float u, v;
		};
	}
}

#endif
