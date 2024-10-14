#include "engine.hpp"
#include <omp.h>
#include "graphics/renderPipeline.hpp"
#include "math/vector3.hpp"
#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_sdlrenderer2.h"
#include "../external/imgui/backends/imgui_impl_sdl2.h"
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
#include <memory>
#include <stdexcept>
#include <thread>

namespace Hiruki {
	Math::Vector3 cameraPosition = Math::Vector3(4, 3, -5.7);
	Engine::Engine(int renderWidth, int renderHeight, int renderScale, float targetFps)
			: renderWidth(renderWidth), renderHeight(renderHeight), renderScale(renderScale),
			windowWidth(renderWidth * renderScale), windowHeight(renderHeight * renderScale),
			m_TargetFps(targetFps), m_Scene(nullptr) {
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

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui_ImplSDL2_InitForSDLRenderer(m_Window, m_Renderer);
		ImGui_ImplSDLRenderer2_Init(m_Renderer);
	}

	Engine::~Engine() {
		if(m_Renderer)
			SDL_DestroyRenderer(m_Renderer);
		if(m_Window)
			SDL_DestroyWindow(m_Window);

		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void Engine::handleEvents() {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: 
					m_Running = false;
				break;
			}
			ImGui_ImplSDL2_ProcessEvent(&event);
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
		omp_set_num_threads(4);
		while (m_Running) {
			auto start = std::chrono::steady_clock::now();

			if(!m_Scene) {
				throw std::runtime_error("[ERROR] There is no active scene set.");
			}

			m_Scene->Update(m_DeltaTime);
			this->handleEvents();
			this->update();
			this->render();

			this->limitFramerate(start);
		}
	}
	
	void Engine::update() {
		// for(int i = 0; i < m_Meshes.size(); i++) {
		// 	m_Meshes[i].rotation.x += 45 * m_DeltaTime;
		// 	m_Meshes[i].rotation.y += 45 * m_DeltaTime;
		// 	m_Meshes[i].rotation.z += 45 * m_DeltaTime;
		// }

		// Left cube
		// m_Meshes[0].rotation.y += 45 * m_DeltaTime;
		// cameraPosition.y = 10;
		//cameraPosition.x += 1 * m_DeltaTime;
		cameraPosition.z += 1 * m_DeltaTime;
		cameraPosition.x += .5 * m_DeltaTime;
		// m_Meshes[0].translation.x = -3;

		// // Middle cube
		// m_Meshes[1].translation.z = 4;
		// 
		// // Right cube
		// m_Meshes[2].translation.x = 5;
		// m_Meshes[2].translation.z = 10;

	}

	void Engine::render() {
		SDL_SetRenderDrawColor(m_Renderer, 0, 0, 255, 255);
		SDL_RenderClear(m_Renderer);
		
		m_RenderPipeline.render(m_Meshes);
		m_Meshes.clear();

		SDL_RenderCopy(m_Renderer, m_RenderPipeline.pixelBufferTexture(), NULL, NULL);

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Debug");
		ImGui::Text("FPS: %.2f", m_Fps);
		ImGui::End();
		ImGui::Render();

		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_Renderer);
		
		SDL_RenderPresent(m_Renderer);
	}
}
