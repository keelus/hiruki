#include "engine.hpp"
#include "graphics/texture.hpp"
#include "graphics/mesh.hpp"
#include <iostream>
#include <memory>

int main(void) {
	std::cout << "hello!" << std::endl;
	Hiruki::Engine engine(320*4, 200*4, 1, 144);

	std::shared_ptr<Hiruki::Graphics::Texture> texture = std::make_shared<Hiruki::Graphics::Texture>("tiles.png");
	uint32_t color = texture->pickColor(0, 0);


	Hiruki::Graphics::Mesh cube = Hiruki::Graphics::Mesh::defaultCube();
	cube.setTexture(texture);

	engine.addMesh(cube);
	engine.addMesh(cube);
	engine.addMesh(cube);

	engine.run();
}
