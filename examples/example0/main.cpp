#include "engine.hpp"
#include "graphics/texture.hpp"
#include "graphics/mesh.hpp"
#include <iostream>
#include <memory>

int main(void) {
	std::cout << "hello!" << std::endl;
	// Hiruki::Engine engine(320*2, 200*2, 3, 144);
	Hiruki::Engine engine(1800, 1000, 1, 144);

	Hiruki::Graphics::Mesh floor = Hiruki::Graphics::Mesh::loadFromFile("assets/floor.obj");
	std::shared_ptr<Hiruki::Graphics::Texture> floorTex = std::make_shared<Hiruki::Graphics::Texture>("assets/textures/floor.png");
	floor.translation.z = 0;
	floor.setTexture(floorTex);

	Hiruki::Graphics::Mesh cop = Hiruki::Graphics::Mesh::loadFromFile("assets/cop.obj");
	std::shared_ptr<Hiruki::Graphics::Texture> copTex = std::make_shared<Hiruki::Graphics::Texture>("assets/textures/cop.png");
	cop.translation = Hiruki::Math::Vector3(1.16538, 1.0405, -1.41271);
	cop.setTexture(copTex);

	Hiruki::Graphics::Mesh car = Hiruki::Graphics::Mesh::loadFromFile("assets/car.obj");
	car.translation = Hiruki::Math::Vector3(3.71223, 0.741519, -2.27015);
	std::shared_ptr<Hiruki::Graphics::Texture> carTex = std::make_shared<Hiruki::Graphics::Texture>("assets/textures/car.png");
	//std::shared_ptr<Hiruki::Graphics::Texture> carLightsTex = std::make_shared<Hiruki::Graphics::Texture>("assets/textures/car_lights.png");
	car.rotation.y = 22.411;
	car.setTexture(carTex);

	Hiruki::Graphics::Mesh cat = Hiruki::Graphics::Mesh::loadFromFile("assets/cat.obj");
	cat.translation = Hiruki::Math::Vector3(-2.1043, 0.175702 , -2.3347);
	cat.rotation.y = -30.8862;
	std::shared_ptr<Hiruki::Graphics::Texture> catTex = std::make_shared<Hiruki::Graphics::Texture>("assets/textures/cat.png");
	cat.setTexture(catTex);

	Hiruki::Graphics::Mesh garage = Hiruki::Graphics::Mesh::loadFromFile("assets/garage.obj");
	garage.translation = Hiruki::Math::Vector3(0.970843, 2.37479, 4.69693);
	std::shared_ptr<Hiruki::Graphics::Texture> garageTex = std::make_shared<Hiruki::Graphics::Texture>("assets/textures/garage.png");
	garage.setTexture(garageTex);

	std::shared_ptr<Hiruki::Graphics::Texture> barrierTex = std::make_shared<Hiruki::Graphics::Texture>("assets/textures/barrier.png");
	Hiruki::Graphics::Mesh barrier1 = Hiruki::Graphics::Mesh::loadFromFile("assets/barrier.obj");
	barrier1.translation = Hiruki::Math::Vector3(-4.79541, 0.427793, 0.868673);
	barrier1.rotation.y = 71;
	barrier1.setTexture(barrierTex);

	Hiruki::Graphics::Mesh barrier2 = Hiruki::Graphics::Mesh::loadFromFile("assets/barrier.obj");
	barrier2.translation = Hiruki::Math::Vector3(4.82119, 0.427793, 2.0027);
	barrier2.rotation.y = 95.4334;
	barrier2.setTexture(barrierTex);

	std::shared_ptr<Hiruki::Graphics::Texture> barrelTex = std::make_shared<Hiruki::Graphics::Texture>("assets/textures/barrel.png");
	// std::shared_ptr<Hiruki::Graphics::Texture> barrelWaterTex = std::make_shared<Hiruki::Graphics::Texture>("assets/textures/barrel_water.png");
	Hiruki::Graphics::Mesh barrel = Hiruki::Graphics::Mesh::loadFromFile("assets/barrel.obj");
	barrel.translation = Hiruki::Math::Vector3(-5.23461, 0.668961, -0.870148);
	barrel.setTexture(barrelTex);

	engine.addMesh(floor);
	engine.addMesh(cop);
	engine.addMesh(garage);
	engine.addMesh(car);
	engine.addMesh(barrier1);
	engine.addMesh(barrier2);
	engine.addMesh(barrel);
	engine.addMesh(cat);

	engine.run();
}
