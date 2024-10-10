#ifndef HIRUKI_GRAPHICS_MESH_H
#define HIRUKI_GRAPHICS_MESH_H

#include "graphics/texCoord.hpp"
#include "graphics/texture.hpp"
#include "math/vector3.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <array>

namespace Hiruki {
	namespace Graphics {
		class Mesh {
			public:
				class Face {
					public:
						constexpr Face() {};
						inline Face(const Math::Vector3i &vertexIndices, const std::array<TexCoord, 3> &texCoords)
								: vertexIndices(vertexIndices), texCoords(texCoords) {}
						inline Face(const Math::Vector3i &vertexIndices, uint32_t color)
								: vertexIndices(vertexIndices), color(color) {};

						Math::Vector3i vertexIndices;
						uint32_t color;
						std::array<TexCoord, 3> texCoords;
				};

				Mesh() : scale(Math::Vector3::one()) {
					texture = nullptr;
				};

				static Mesh loadFromFile(std::string filename);
				static Mesh defaultCube();

				void setTexture(std::shared_ptr<Graphics::Texture> texture) {
					this->texture = texture;
				}

				std::vector<Math::Vector3> vertices;
				std::vector<Face> faces;
				Math::Vector3 scale;
				Math::Vector3 rotation;
				Math::Vector3 translation;

				std::shared_ptr<Graphics::Texture> texture;
		};
	}
}

#endif
