#pragma once

#include "SDL_events.h"
#include "SDL_scancode.h"
#include "math/vector3.hpp"
#include "scene.hpp"
#include "engine.hpp"
#include "graphics/mesh.hpp"
#include <memory>
#include <vector>

class MainScene : public Hiruki::Scene {
public:
	MainScene() {};

	void setup() {
		m_Triangle = Hiruki::Graphics::Mesh::empty();

		// Add triangle's vertices
		m_Triangle->vertices.emplace_back(0, 1, 0);
		m_Triangle->vertices.emplace_back(-1.25, -1, 0);
		m_Triangle->vertices.emplace_back(1.25, -1, 0);

		// Add triangle's face
		m_Triangle->faces.emplace_back(Hiruki::Math::Vector3i(1, 2, 3), 0xFFFFFFFF);

		m_Camera.setPosition(Hiruki::Math::Vector3(0, 0, -3));
		m_Camera.setTarget(Hiruki::Math::Vector3(0, 0, 0));
	};

	void update(float deltaTime) {
		engine.lock().get()->addMesh(*m_Triangle);
	}

	void handleEvents(float deltaTime) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: 
					engine.lock()->exit();
				break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.scancode) {
						case SDL_SCANCODE_ESCAPE:
							engine.lock()->exit();
						break;
						case SDL_SCANCODE_W:
							engine.lock()->setRenderWireframeEnabled(!engine.lock()->getRenderWireframeEnabled());
						break;
						default:
						break;
					}
				break;
			}
		}
	}

private:
	std::unique_ptr<Hiruki::Graphics::Mesh> m_Triangle;
};
