#ifndef HIRUKI_ENGINE_H
#define HIRUKI_ENGINE_H

#include "SDL_ttf.h"
#include "graphics/mesh.hpp"
#include "graphics/renderPipeline.hpp"
#include "scene.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <chrono>
#include <functional>
#include <memory>
#include <omp.h>
#include <vector>

namespace Hiruki {
	class Engine : public std::enable_shared_from_this<Engine> {
		public:
			Engine(int windowWidth, int windowHeight, int renderWidth, int renderHeight, float targetFps);
			Engine(int renderWidth, int renderHeight, int renderScale, float targetFps);
			~Engine();

			inline void setScene(std::shared_ptr<Scene> scene) {
				m_Scene = scene;
				scene->engine = shared_from_this();
				scene->setup();
			};
			
			inline void addMesh(const Graphics::Mesh &mesh) { m_Meshes.push_back(mesh); }

			inline void setRenderDrawMode(Graphics::RenderPipeline::DrawMode drawMode) {
				m_RenderPipeline.setDrawMode(drawMode);
			}

			inline void setRenderShadingMode(Graphics::RenderPipeline::ShadingMode shadingMode) {
				m_RenderPipeline.setShadingMode(shadingMode);
			}

			inline void setRenderWireframeEnabled(bool enabled) {
				m_RenderPipeline.setWireframeEnabled(enabled);
			}

			inline void setRenderWireframeColor(uint32_t color) {
				m_RenderPipeline.setWireframeColor(color);
			}

			inline Graphics::RenderPipeline::DrawMode getRenderDrawMode() const {
				return m_RenderPipeline.getDrawMode();
			}

			inline Graphics::RenderPipeline::ShadingMode getRenderShadingMode() const  {
				return m_RenderPipeline.getShadingMode();
			}

			inline bool getRenderWireframeEnabled() const {
				return m_RenderPipeline.getWireframeEnabled();
			}

			inline uint32_t getRenderWireframeColor() const {
				return m_RenderPipeline.getWireframeColor();
			}

			inline void setDrawFps(bool drawFps) {
				m_DrawFps = drawFps;
			}

			inline bool getDrawFps() const {
				return m_DrawFps;
			}

			inline Math::Vector2 getWindowSize() const {
				return Math::Vector2(m_WindowWidth, m_WindowHeight);
			}

			inline void setWindowSize(int windowWidth, int windowHeight) {
				m_WindowWidth = windowWidth;
				m_WindowHeight = windowHeight;

				SDL_SetWindowSize(m_Window, windowWidth, windowHeight);
			}

			inline Math::Vector2 getRenderSize() const {
				return m_RenderPipeline.getSize();
			}

			inline void setRenderSize(int renderWidth, int renderHeight) {
				m_RenderPipeline.setSize(m_Renderer, renderWidth, renderHeight);
			}

			inline void setFpsLimit(int fpsLimit) {
				m_FpsLimit = fpsLimit;
				m_FrameTimeMs = 1000.0 / static_cast<double>(fpsLimit);
			}

			inline void disableFpsLimit() {
				m_FpsLimit = -1;
			}

			// Recommended: between 4 and 8
			inline void enableRasterOptimizations(size_t numThreads) {
				m_RasterThreads = numThreads;
				omp_set_num_threads(numThreads);
			}
			inline void disableRasterOptimizations() {
				m_RasterThreads = 1;
				omp_set_num_threads(1);
			}

			inline void exit() {
				m_Running = false;
			}

			void run();

		private:
			void limitFramerate(std::chrono::steady_clock::time_point frameStart);
			void render();

			// Main attributes
			bool m_Running;
			SDL_Window* m_Window;
			SDL_Renderer* m_Renderer;

			Graphics::RenderPipeline m_RenderPipeline;

			int m_WindowWidth;
			int m_WindowHeight;

			std::shared_ptr<Scene> m_Scene;

			size_t m_RasterThreads;

			// Fps limit
			int m_FpsLimit;
			float m_FrameTimeMs; // Calculated on FpsLimit change

			// Whether to draw or hide the current FPS
			bool m_DrawFps;

			// Stats calculated per-frame
			float m_DeltaTime;
			float m_Fps;
			float m_LastFrameDuration;

			// Meshes to render per-frame
			std::vector<std::reference_wrapper<const Graphics::Mesh>> m_Meshes;

			// Font
			TTF_Font *m_FontAlagard;
	};
}

#endif
