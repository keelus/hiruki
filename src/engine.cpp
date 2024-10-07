#include "engine.hpp"
#include "graphics/renderPipeline.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

namespace Hiruki {
	Engine::Engine(int windowWidth, int windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight) {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			throw std::runtime_error("Error initializing SDL.\n");
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

		SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

		m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
		if (!m_Renderer) {
				throw std::runtime_error("Error creating SDL renderer.\n");
		}

		m_RenderPipeline = Graphics::RenderPipeline(windowWidth, windowHeight, m_Renderer);
		m_Running = true;
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

	void Engine::run() {
		while (m_Running) {
			this->handleEvents();
			this->update();
			this->render();
		}
	}
	
	void Engine::update() {
	}
	void Engine::render() {
		SDL_SetRenderDrawColor(m_Renderer, 0, 0, 255, 255);
		SDL_RenderClear(m_Renderer);
		
		m_RenderPipeline.render(m_Meshes);
		SDL_RenderCopy(m_Renderer, m_RenderPipeline.pixelBufferTexture(), NULL, NULL);
		
		SDL_RenderPresent(m_Renderer);
	}
}
