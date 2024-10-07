#include "engine.hpp"
#include "graphics/mesh.hpp"

int main(void) {
	Hiruki::Engine engine(200, 200, 5, 144);

	Hiruki::Graphics::Mesh cube = Hiruki::Graphics::Mesh::defaultCube();

	engine.addMesh(cube);

	engine.run();
}
