#ifndef HIRUKI_GRAPHICS_MESH_H
#define HIRUKI_GRAPHICS_MESH_H

#include "graphics/material.hpp"
#include "graphics/texCoord.hpp"
#include "math/vector3.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

namespace Hiruki {
	namespace Graphics {
		class Mesh {
			public:
				class Face {
					public:
						inline Face(const Math::Vector3i &vertexIndices, const std::array<TexCoord, 3> &texCoords, const size_t textureIndex)
								: vertexIndices(vertexIndices), texCoords(texCoords), textureIndex(textureIndex) {}
						inline Face(const Math::Vector3i &vertexIndices, uint32_t color)
								: vertexIndices(vertexIndices), color(color) {};

						size_t textureIndex = 0;
						Math::Vector3i vertexIndices = Math::Vector3i(1, 1, 1);
						uint32_t color = 0xFFFFFFFF;
						std::array<TexCoord, 3> texCoords = {TexCoord(0, 0), TexCoord(0,0), TexCoord(0,0)};
				};

				Mesh() : scale(Math::Vector3::one()) {};

				void parseMaterial(std::string objFilename, std::unordered_map<std::string, size_t> materialNames);

				static std::unique_ptr<Mesh> loadFromFile(std::string filename);
				static Mesh defaultCube();
				static std::unique_ptr<Mesh> empty() {
					std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
					return mesh;
				};

				std::vector<Math::Vector3> vertices;
				std::vector<Face> faces;
				Math::Vector3 scale;
				Math::Vector3 rotation;
				Math::Vector3 translation;

				std::unordered_map<size_t, Material> m_Materials = {{0, Material()}}; // Fallback material
		};
	}
}

#endif
