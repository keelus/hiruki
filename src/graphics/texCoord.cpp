#include "texCoord.hpp"
#include "../math/vector2.hpp"

namespace Hiruki {
	namespace Graphics {
		TexCoord::TexCoord(const Math::Vector2 &vector) : u(vector.x), v(vector.y) {}
	}
}
