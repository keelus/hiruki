#include "engine.hpp"
#include "graphics/renderPipeline.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <chrono>
#include <iomanip>
#include <ios>
#include <iostream>
#include <thread>

namespace Hiruki {
	Engine::Engine(int renderWidth, int renderHeight, int renderScale, float targetFps)
			: renderWidth(renderWidth), renderHeight(renderHeight), renderScale(renderScale),
			windowWidth(renderWidth * renderScale), windowHeight(renderHeight * renderScale),
			m_TargetFps(targetFps) {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			throw std::runtime_error("Error initializing SDL.\n");
		}

		if (IMG_Init(0) != 0) {
			throw std::runtime_error("Error initializing SDL_image.\n");
		}

		m_Window = SDL_CreateWindow(
					"Hiruki engine window",
					SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					windowWidth, windowHeight,
					SDL_WINDOW_BORDERLESS
		);
		if (!m_Window) {
			throw std::runtime_error("Error creating SDL window.\n");
		}

		m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
		if (!m_Renderer) {
				throw std::runtime_error("Error creating SDL renderer.\n");
		}

		m_RenderPipeline = Graphics::RenderPipeline(renderWidth, renderHeight, m_Renderer);
		m_Running = true;
		m_DeltaTime = 0;
	}

	Engine::~Engine() {
		if(m_Renderer)
			SDL_DestroyRenderer(m_Renderer);
		if(m_Window)
			SDL_DestroyWindow(m_Window);
	}

	void Engine::handleEvents() {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: 
					m_Running = false;
				break;
			}
		}
	}

	const double TARGET_FPS = 60;
	const double TARGET_FRAME_DURATION = static_cast<double>(1000.0 / TARGET_FPS);
	void Engine::limitFramerate(std::chrono::steady_clock::time_point frameStart) {
    	auto frameEnd = std::chrono::steady_clock::now();
    	float frameDurationMs = std::chrono::duration<float, std::milli>(frameEnd - frameStart).count();

    	if (frameDurationMs < TARGET_FRAME_DURATION) {
    	    float toWaitMs = TARGET_FRAME_DURATION - frameDurationMs;
    	    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(toWaitMs));
    	}

    	frameEnd = std::chrono::steady_clock::now(); // Update end time after waiting
    	m_FrameDuration = std::chrono::duration<float, std::milli>(frameEnd - frameStart).count();
    	m_DeltaTime = m_FrameDuration / 1000.0f;
		m_Fps = 1000.0f / m_FrameDuration;
	}

	void Engine::run() {
		while (m_Running) {
			auto start = std::chrono::steady_clock::now();

			this->handleEvents();
			this->update();
			this->render();

			this->limitFramerate(start);

			std::cout << std::fixed << std::setprecision(4);
			std::cout << "Render time: " << m_FrameDuration << "ms [" << m_Fps << "fps] [dt: " << m_DeltaTime << "]" << std::endl;
		}
	}
	
	void Engine::update() {
	}

	void Engine::render() {
		SDL_SetRenderDrawColor(m_Renderer, 0, 0, 255, 255);
		SDL_RenderClear(m_Renderer);
		
		for(int i = 0; i < 3; i++) {
			m_Meshes[i].rotation.x += 45 * m_DeltaTime;
			m_Meshes[i].rotation.y += 45 * m_DeltaTime;
			m_Meshes[i].rotation.z += 45 * m_DeltaTime;
		}

		// Left cube
		m_Meshes[0].translation.z = 10;
		m_Meshes[0].translation.x = -3;

		// Middle cube
		m_Meshes[1].translation.z = 7;
		
		// Right cube
		m_Meshes[2].translation.x = 5;
		m_Meshes[2].translation.z = 20;

		m_RenderPipeline.render(m_Meshes);

		SDL_RenderCopy(m_Renderer, m_RenderPipeline.pixelBufferTexture(), NULL, NULL);
		
		SDL_RenderPresent(m_Renderer);

	}
}
