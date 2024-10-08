#ifndef HIRUKI_GRAPHICS_MESH_H
#define HIRUKI_GRAPHICS_MESH_H

#include "graphics/texCoord.hpp"
#include "graphics/texture.hpp"
#include "math/vector3.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Hiruki {
	namespace Graphics {
		class Mesh {
			public:
				class Face {
					public:
						Face() {};
						Face(int vertexIndex0, int vertexIndex1, int vertexIndex2, uint32_t color,
							TexCoord vertexTexcoord0, TexCoord vertexTexcoord1, TexCoord vertexTexcoord2) 
								: vertexIndex0(vertexIndex0), vertexIndex1(vertexIndex1), vertexIndex2(vertexIndex2), color(color),
								vertexTexcoord0(vertexTexcoord0), vertexTexcoord1(vertexTexcoord1), vertexTexcoord2(vertexTexcoord2) {}

						int vertexIndex0;
						int vertexIndex1;
						int vertexIndex2;
						uint32_t color;
						TexCoord vertexTexcoord0;
						TexCoord vertexTexcoord1;
						TexCoord vertexTexcoord2;
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
