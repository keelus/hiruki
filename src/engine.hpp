#ifndef HIRUKI_ENGINE_H
#define HIRUKI_ENGINE_H

#include "graphics/mesh.hpp"
#include "graphics/renderPipeline.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <vector>

namespace Hiruki {
	class Engine {
		public:
			Engine(int windowWidth, int windowHeight);
			~Engine();
			
			inline void addMesh(Graphics::Mesh mesh) { m_Meshes.push_back(mesh); }

			void run();
			void handleEvents();
			void update();
			void render();

			const int windowWidth;
			const int windowHeight;
		private:
			bool m_Running;
			SDL_Window* m_Window;
			SDL_Renderer* m_Renderer;
			
			std::vector<Graphics::Mesh> m_Meshes;
			Graphics::RenderPipeline m_RenderPipeline;
	};
}

#endif
