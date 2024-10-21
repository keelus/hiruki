#pragma once

#include "SDL_events.h"
#include "SDL_scancode.h"
#include "scene.hpp"
#include "engine.hpp"
#include "graphics/mesh.hpp"
#include <memory>
#include <vector>

class MainScene : public Hiruki::Scene {
public:
	MainScene() {};

	void setup() {
		// Floor object
		std::unique_ptr<Hiruki::Graphics::Mesh> floor = Hiruki::Graphics::Mesh::loadFromFile("assets/floor.obj");
		floor->translation.z = 0;

		// Cop object
		std::unique_ptr<Hiruki::Graphics::Mesh> cop = Hiruki::Graphics::Mesh::loadFromFile("assets/cop.obj");
		cop->translation = Hiruki::Math::Vector3(1.16538, 1.0405, -1.41271);

		// Car object
		std::unique_ptr<Hiruki::Graphics::Mesh> car = Hiruki::Graphics::Mesh::loadFromFile("assets/car.obj");
		car->translation = Hiruki::Math::Vector3(3.71223, 0.741519, -2.27015);
		car->rotation.y = 22.411;

		// Cat object
		std::unique_ptr<Hiruki::Graphics::Mesh> cat = Hiruki::Graphics::Mesh::loadFromFile("assets/cat.obj");
		cat->translation = Hiruki::Math::Vector3(-2.1043, 0.175702 , -2.3347);
		cat->rotation.y = -30.8862;

		// Garage object
		std::unique_ptr<Hiruki::Graphics::Mesh> garage = Hiruki::Graphics::Mesh::loadFromFile("assets/garage.obj");
		garage->translation = Hiruki::Math::Vector3(0.970843, 2.37479, 4.69693);

		// Barrier object 1
		std::unique_ptr<Hiruki::Graphics::Mesh> barrier1 = Hiruki::Graphics::Mesh::loadFromFile("assets/barrier.obj");
		barrier1->translation = Hiruki::Math::Vector3(-4.79541, 0.427793, 0.868673);
		barrier1->rotation.y = 71;

		// Barrier object 2
		std::unique_ptr<Hiruki::Graphics::Mesh> barrier2 = Hiruki::Graphics::Mesh::loadFromFile("assets/barrier.obj");
		barrier2->translation = Hiruki::Math::Vector3(4.82119, 0.427793, 2.0027);
		barrier2->rotation.y = 95.4334;

		// Barrel object
		std::unique_ptr<Hiruki::Graphics::Mesh> barrel = Hiruki::Graphics::Mesh::loadFromFile("assets/barrel.obj");
		barrel->translation = Hiruki::Math::Vector3(-5.23461, 0.668961, -0.870148);

		// Move all the models into the objects vector
		m_Objects.push_back(std::move(floor));
		m_Objects.push_back(std::move(cop));
		m_Objects.push_back(std::move(garage));
		m_Objects.push_back(std::move(car));
		m_Objects.push_back(std::move(barrier1));
		m_Objects.push_back(std::move(barrier2));
		m_Objects.push_back(std::move(barrel));
		m_Objects.push_back(std::move(cat));

		// Camera starting state
		m_Camera.setPosition(Hiruki::Math::Vector3(4, 3, -5.7));
		m_Camera.setTarget(Hiruki::Math::Vector3(0, 0, 0));
	};

	void update(float deltaTime) {
		// Move the camera along the scene
		m_Camera.getPosition().x += 1 * deltaTime;
		m_Camera.getPosition().z += 1 * deltaTime;

		// Render all the objects
		for(const std::unique_ptr<Hiruki::Graphics::Mesh> &mesh : m_Objects) {
			engine.lock().get()->addMesh(*mesh);
		}
	}

	void handleEvents(float deltaTime) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: // Handle engine exit
					engine.lock()->exit();
				break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.scancode) {
						case SDL_SCANCODE_ESCAPE: // Handle engine exit
							engine.lock()->exit();
						break;
						default:
						break;
					}
				break;
			}
		}
	}

private:
	std::vector<std::unique_ptr<Hiruki::Graphics::Mesh>> m_Objects;
};
