#ifndef HIRUKI_ENGINE_H
#define HIRUKI_ENGINE_H

#include "graphics/mesh.hpp"
#include "graphics/renderPipeline.hpp"
#include "scene.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <chrono>
#include <memory>
#include <vector>

namespace Hiruki {
	class Engine : public std::enable_shared_from_this<Engine> {
		public:
			Engine(int renderWidth, int renderHeight, int renderScale, float targetFps);
			~Engine();

			void setScene(std::shared_ptr<Scene> scene) {
				m_Scene = scene;
				scene->engine = shared_from_this();
				scene->Setup();
			};
			
			inline void addMesh(Graphics::Mesh mesh) { m_Meshes.push_back(mesh); }

			void run();
			void limitFramerate(std::chrono::steady_clock::time_point frameStart);
			void handleEvents();
			void update();
			void render();

			const int windowWidth;
			const int windowHeight;
			const int renderScale;
			const int renderWidth;
			const int renderHeight;

			const float m_TargetFps;
		private:
			bool m_Running;
			SDL_Window* m_Window;
			SDL_Renderer* m_Renderer;

			std::shared_ptr<Scene> m_Scene;

			float m_DeltaTime;
			float m_Fps;
			float m_FrameDuration;

			std::vector<Graphics::Mesh> m_Meshes;
			Graphics::RenderPipeline m_RenderPipeline;
	};
}

#endif
