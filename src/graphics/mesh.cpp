#include "mesh.hpp"
#include "math/vector3.hpp"
#include <cstdio>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <utility>

namespace Hiruki {
	namespace Graphics {
		Mesh Mesh::loadFromFile(std::string filename) {
			Mesh mesh;
			mesh.scale = Math::Vector3::one();
		
			if(!filename.ends_with(".obj")) {
				throw std::invalid_argument("Only OBJ format supported at the moment.");
			}

			std::ifstream meshFile(filename);
			std::string line;
			std::vector<TexCoord> texCoords;

			while(getline(meshFile, line)) {
				if(line.starts_with("v ")) {
					float x, y, z;
					std::sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
					mesh.vertices.emplace_back(x, y, z);
				} else if(line.starts_with("vt ")) {
					float x, y;
					std::sscanf(line.c_str(), "vt %f %f", &x, &y);
					texCoords.emplace_back(x, y);
				} else if(line.starts_with("f ")) {
					Math::Vector3i vertexIndices;
					Math::Vector3i textureIndices;
					Math::Vector3i normalIndices;

					std::sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
						&vertexIndices.x, &textureIndices.x, &normalIndices.x,
						&vertexIndices.y, &textureIndices.y, &normalIndices.y,
						&vertexIndices.z, &textureIndices.z, &normalIndices.z
					);
					
					// Useful when loading a right-handed model
					bool invertFaces = true;
					if(invertFaces) {
						std::swap(vertexIndices.y, vertexIndices.z);
						std::swap(textureIndices.y, textureIndices.z);
						std::swap(normalIndices.y, normalIndices.z);
					}

					mesh.faces.emplace_back(
						vertexIndices,
						std::array<TexCoord, 3> {
							texCoords.at(textureIndices.x-1),
							texCoords.at(textureIndices.y-1),
							texCoords.at(textureIndices.z-1)
						}
					);
				}
				std::cout << line << std::endl;
			}

			meshFile.close();

			return mesh;
		}

		Mesh Mesh::defaultCube() {
			Mesh mesh;
			mesh.scale = Math::Vector3::one();

			mesh.vertices = {
				Math::Vector3(-1, -1, -1),
				Math::Vector3(1, -1, -1),
				Math::Vector3(-1, 1, -1),
				Math::Vector3(1, 1, -1),
				Math::Vector3(-1, -1, 1),
				Math::Vector3(1, -1, 1),
				Math::Vector3(-1, 1, 1),
				Math::Vector3(1, 1, 1),
			};

			mesh.faces = {
				// Front face
				Mesh::Face(Math::Vector3i(3, 1, 4), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}),
				Mesh::Face(Math::Vector3i(1, 2, 4), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}),
				// Right face
				Mesh::Face(Math::Vector3i(4, 2, 8), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}),
				Mesh::Face(Math::Vector3i(2, 6, 8), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}),
				// Back face
				Mesh::Face(Math::Vector3i(8, 6, 7), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}),
				Mesh::Face(Math::Vector3i(6, 5, 7), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}),
				// Left face
				Mesh::Face(Math::Vector3i(7, 5, 3), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}),
				Mesh::Face(Math::Vector3i(5, 1, 3), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}),
				// Top face
				Mesh::Face(Math::Vector3i(7, 3, 8), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}),
				Mesh::Face(Math::Vector3i(3, 4, 8), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}),
				// Bottom face
				Mesh::Face(Math::Vector3i(1, 5, 2), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}),
				Mesh::Face(Math::Vector3i(5, 6, 2), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}),
			};
			
			return mesh;
		}
	}
}
