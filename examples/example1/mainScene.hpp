#pragma once

#include "SDL_events.h"
#include "SDL_scancode.h"
#include "math/vector3.hpp"
#include "scene.hpp"
#include "engine.hpp"
#include "graphics/mesh.hpp"
#include <memory>

class MainScene : public Hiruki::Scene {
public:
	MainScene() {};

	void setup() {
		m_Cube = Hiruki::Graphics::Mesh::loadFromFile("./assets/cube.obj");
		m_Cube->translation.z = 5;

		m_Camera.setPosition(Hiruki::Math::Vector3(0, 0, -3));
		m_Camera.setTarget(Hiruki::Math::Vector3(0, 0, 0));

		m_LightDirection = Hiruki::Math::Vector3(-1, 0, 0);
	};

	void update(float deltaTime) {
		m_Cube->rotation.x += 30 * deltaTime;
		m_Cube->rotation.y += 30 * deltaTime;
		m_Cube->rotation.z += 30 * deltaTime;

		engine.lock().get()->addMesh(*m_Cube);
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
						case SDL_SCANCODE_T:
							switch(engine.lock()->getRenderDrawMode()) {
								case Hiruki::Graphics::RenderPipeline::DrawMode::SOLID:
									engine.lock()->setRenderDrawMode(Hiruki::Graphics::RenderPipeline::DrawMode::TEXTURED);
								break;
								case Hiruki::Graphics::RenderPipeline::DrawMode::TEXTURED:
									engine.lock()->setRenderDrawMode(Hiruki::Graphics::RenderPipeline::DrawMode::GRADIENT);
								break;
								case Hiruki::Graphics::RenderPipeline::DrawMode::GRADIENT:
									engine.lock()->setRenderDrawMode(Hiruki::Graphics::RenderPipeline::DrawMode::SOLID);
								break;
							}
						break;
						case SDL_SCANCODE_S:
							switch(engine.lock()->getRenderShadingMode()) {
								case Hiruki::Graphics::RenderPipeline::ShadingMode::NONE:
									engine.lock()->setRenderShadingMode(Hiruki::Graphics::RenderPipeline::ShadingMode::FLAT);
								break;
								case Hiruki::Graphics::RenderPipeline::ShadingMode::FLAT:
									engine.lock()->setRenderShadingMode(Hiruki::Graphics::RenderPipeline::ShadingMode::GORAUD);
								break;
								case Hiruki::Graphics::RenderPipeline::ShadingMode::GORAUD:
									engine.lock()->setRenderShadingMode(Hiruki::Graphics::RenderPipeline::ShadingMode::NONE);
								break;
							}
						break;
						default:
						break;
					}
				break;
			}
		}
	}

private:
	std::unique_ptr<Hiruki::Graphics::Mesh> m_Cube;
};
