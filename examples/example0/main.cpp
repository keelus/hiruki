#include "engine.hpp"
#include "graphics/mesh.hpp"

int main(void) {
	Hiruki::Engine engine(1000, 1000);

	Hiruki::Graphics::Mesh cube = Hiruki::Graphics::Mesh::defaultCube();

	engine.addMesh(cube);

	engine.run();
}
