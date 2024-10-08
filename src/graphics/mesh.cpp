#include "mesh.hpp"
#include "math/vector3.hpp"

namespace Hiruki {
	namespace Graphics {
		Mesh Mesh::loadFromFile(std::string filename) {
			Mesh mesh;

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
				Mesh::Face(3, 1, 4, 0xFF0000FF, TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)),
				Mesh::Face(1, 2, 4, 0xFF0000FF, TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)),
				// Right face
				Mesh::Face(4, 2, 8, 0xFF0000FF, TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)),
				Mesh::Face(2, 6, 8, 0xFF0000FF, TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)),
				// Back face
				Mesh::Face(8, 6, 7, 0xFF0000FF, TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)),
				Mesh::Face(6, 5, 7, 0xFF0000FF, TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)),
				// Left face
				Mesh::Face(7, 5, 3, 0xFF0000FF, TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)),
				Mesh::Face(5, 1, 3, 0xFF0000FF, TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)),
				// Top face
				Mesh::Face(7, 3, 8, 0xFF0000FF, TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)),
				Mesh::Face(3, 4, 8, 0xFF0000FF, TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)),
				// Bottom face
				Mesh::Face(1, 5, 2, 0xFF0000FF, TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)),
				Mesh::Face(5, 6, 2, 0xFF0000FF, TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)),
			};
			
			return mesh;
		}
	}
}
