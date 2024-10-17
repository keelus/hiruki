#include "engine.hpp"
#include <cstdio>
#include <format>
#include <omp.h>
#include "SDL_rwops.h"
#include "graphics/renderPipeline.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <thread>

extern const char ALAGARD_RAW[];
extern const size_t ALAGARD_RAW_len;

namespace Hiruki {
	Engine::Engine(int windowWidth, int windowHeight, int renderWidth, int renderHeight, float targetFps)
			: m_WindowWidth(windowWidth), m_WindowHeight(windowHeight),
				m_Scene(nullptr), m_TargetFps(targetFps) { 
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			throw std::runtime_error("Error initializing SDL.\n");
		}

		if (IMG_Init(0) != 0) {
			throw std::runtime_error("Error initializing SDL_image.\n");
		}

		if (TTF_Init() != 0) {
			throw std::runtime_error("Error initializing SDL_ttf.\n");
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

		m_DrawFps = false;

		SDL_RWops *fontAlagardMem= SDL_RWFromConstMem(ALAGARD_RAW, ALAGARD_RAW_len);
		m_FontAlagard = TTF_OpenFontRW(fontAlagardMem, 1, 32);
	}

	Engine::Engine(int renderWidth, int renderHeight, int renderScale, float targetFps)
			: Hiruki::Engine(renderWidth * renderScale, renderHeight * renderScale, renderWidth, renderHeight, targetFps){}

	Engine::~Engine() {
		if(m_Renderer)
			SDL_DestroyRenderer(m_Renderer);
		if(m_Window)
			SDL_DestroyWindow(m_Window);

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
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
		omp_set_num_threads(4);
		while (m_Running) {
			auto start = std::chrono::steady_clock::now();

			if(!m_Scene) {
				throw std::runtime_error("[ERROR] There is no active scene set.");
			}

			m_Scene->handleEvents(m_DeltaTime);
			m_Scene->update(m_DeltaTime);
			this->update();
			this->render();

			this->limitFramerate(start);
		}
	}
	
	void Engine::update() {
	}

	void Engine::render() {
		SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_Renderer);
		
		m_RenderPipeline.render(m_Meshes, m_Scene->getCamera());
		m_Meshes.clear();

		SDL_RenderCopy(m_Renderer, m_RenderPipeline.pixelBufferTexture(), NULL, NULL);


		SDL_Surface* fpsTextSurface = TTF_RenderText_Solid(m_FontAlagard, std::format("FPS: {:.2f}", m_Fps).c_str(), {255, 255, 255, 255}); 
		SDL_Texture* fpsTextTexture = SDL_CreateTextureFromSurface(m_Renderer, fpsTextSurface);
		SDL_Rect fpsTextRect = {10, 10, fpsTextSurface->w, fpsTextSurface->h};
		SDL_RenderCopy(m_Renderer, fpsTextTexture, NULL, &fpsTextRect);
		
		SDL_RenderPresent(m_Renderer);
	}
}
