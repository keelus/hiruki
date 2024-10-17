#ifndef HIRUKI_GRAPHICS_RENDER_PIPELINE_H
#define HIRUKI_GRAPHICS_RENDER_PIPELINE_H

#include "graphics/mesh.hpp"
#include "graphics/triangle.hpp"
#include "math/vector2.hpp"
#include "math/vector3.hpp"
#include "scene.hpp"
#include <SDL2/SDL_render.h>
#include <vector>

namespace Hiruki {
	namespace Graphics {
		class RenderPipeline {
			public:
				enum class DrawMode {
					SOLID,
					GRADIENT, // RGB interpolated triangle
					TEXTURED,
				};
				enum class ShadingMode {
					NONE,
					FLAT,
					GORAUD
				};
				RenderPipeline() {}
				RenderPipeline(int renderWidth, int renderHeight, SDL_Renderer *renderer);

				~RenderPipeline();
				
				void render(const std::vector<std::reference_wrapper<const Mesh>> &meshes,
							const Scene::Camera camera);

				SDL_Texture * pixelBufferTexture() {
					return m_PixelBufferTexture;
				}

				RenderPipeline& operator=(RenderPipeline&& other) noexcept {
					if (this != &other) {
						m_PixelBufferTexture = other.m_PixelBufferTexture;
						other.m_PixelBufferTexture = nullptr;

						m_PixelBufferWidth = other.m_PixelBufferWidth; 
						m_PixelBufferHeight = other.m_PixelBufferHeight; 

						m_PixelBuffer = std::move(other.m_PixelBuffer);
						m_DepthBuffer = std::move(other.m_DepthBuffer);

						m_DrawMode = other.m_DrawMode;
						m_ShadingMode = other.m_ShadingMode;

						m_WireframeEnabled = other.m_WireframeEnabled;
						m_WireframeColor = other.m_WireframeColor;
					}
					return *this;
				}

				void setSize(SDL_Renderer *renderer, int renderWidth, int renderHeight);
				Math::Vector2 getSize() const { return Math::Vector2(m_PixelBufferWidth, m_PixelBufferHeight); }

				void setDrawMode(DrawMode drawMode) { m_DrawMode = drawMode; }
				void setShadingMode(ShadingMode shadingMode) { m_ShadingMode = shadingMode; }
				void setWireframeEnabled(bool enabled) { m_WireframeEnabled = enabled; }
				void setWireframeColor(uint32_t color) { m_WireframeColor = color; }

				DrawMode getDrawMode() const { return m_DrawMode; };
				ShadingMode getShadingMode() const { return m_ShadingMode; };
				bool getWireframeEnabled() const { return m_WireframeEnabled; }
				uint32_t getWireframeColor() const { return m_WireframeColor; }
				
				void drawTriangle(const Triangle &triangle);
				void drawTriangleParallel(const Triangle &triangle);

				void drawTriangleWireframe(const Triangle &triangle, uint32_t color);
				void drawLine(Math::Vector4 v0, Math::Vector4 v1, uint32_t color);

				void drawPixel(int x, int y, uint32_t color);

			private:
				SDL_Texture* m_PixelBufferTexture;
				std::vector<uint32_t> m_PixelBuffer;
				std::vector<float> m_DepthBuffer;

				int m_PixelBufferWidth;
				int m_PixelBufferHeight;

				DrawMode m_DrawMode;
				ShadingMode m_ShadingMode;

				bool m_WireframeEnabled;
				uint32_t m_WireframeColor;

		};
	}
}

#endif
