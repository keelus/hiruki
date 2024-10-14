#pragma once

#include "scene.hpp"
#include "engine.hpp"
#include "graphics/mesh.hpp"
#include <memory>
#include <vector>

class MainScene : public Hiruki::Scene {
public:
	MainScene() {};

	void Setup() {
		std::unique_ptr<Hiruki::Graphics::Mesh> floor = Hiruki::Graphics::Mesh::loadFromFile("assets/floor.obj");
		floor->translation.z = 0;

		std::unique_ptr<Hiruki::Graphics::Mesh> cop = Hiruki::Graphics::Mesh::loadFromFile("assets/cop.obj");
		cop->translation = Hiruki::Math::Vector3(1.16538, 1.0405, -1.41271);

		std::unique_ptr<Hiruki::Graphics::Mesh> car = Hiruki::Graphics::Mesh::loadFromFile("assets/car.obj");
		car->translation = Hiruki::Math::Vector3(3.71223, 0.741519, -2.27015);
		car->rotation.y = 22.411;

		std::unique_ptr<Hiruki::Graphics::Mesh> cat = Hiruki::Graphics::Mesh::loadFromFile("assets/cat.obj");
		cat->translation = Hiruki::Math::Vector3(-2.1043, 0.175702 , -2.3347);
		cat->rotation.y = -30.8862;

		std::unique_ptr<Hiruki::Graphics::Mesh> garage = Hiruki::Graphics::Mesh::loadFromFile("assets/garage.obj");
		garage->translation = Hiruki::Math::Vector3(0.970843, 2.37479, 4.69693);

		std::unique_ptr<Hiruki::Graphics::Mesh> barrier1 = Hiruki::Graphics::Mesh::loadFromFile("assets/barrier.obj");
		barrier1->translation = Hiruki::Math::Vector3(-4.79541, 0.427793, 0.868673);
		barrier1->rotation.y = 71;

		std::unique_ptr<Hiruki::Graphics::Mesh> barrier2 = Hiruki::Graphics::Mesh::loadFromFile("assets/barrier.obj");
		barrier2->translation = Hiruki::Math::Vector3(4.82119, 0.427793, 2.0027);
		barrier2->rotation.y = 95.4334;

		std::unique_ptr<Hiruki::Graphics::Mesh> barrel = Hiruki::Graphics::Mesh::loadFromFile("assets/barrel.obj");
		barrel->translation = Hiruki::Math::Vector3(-5.23461, 0.668961, -0.870148);

		m_Objects.push_back(std::move(floor));
		m_Objects.push_back(std::move(cop));
		m_Objects.push_back(std::move(garage));
		m_Objects.push_back(std::move(car));
		m_Objects.push_back(std::move(barrier1));
		m_Objects.push_back(std::move(barrier2));
		m_Objects.push_back(std::move(barrel));
		m_Objects.push_back(std::move(cat));
	};

	void Update(float deltaTime) {
		for(const std::unique_ptr<Hiruki::Graphics::Mesh> &mesh : m_Objects) {
			engine.lock().get()->addMesh(*mesh);
		}
	}

private:
	std::vector<std::unique_ptr<Hiruki::Graphics::Mesh>> m_Objects;
};
