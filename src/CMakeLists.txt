add_library(hiruki STATIC
	math/vector2.cpp
	math/vector3.cpp
	math/vector4.cpp

	graphics/texCoord.cpp
	graphics/mesh.cpp
	graphics/renderPipeline.cpp
	graphics/texture.cpp
	graphics/clipping.cpp

	engine.cpp
)

set(IMGUI_DIR ../external/imgui/)

add_library(imgui
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
	${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_include_directories(imgui PUBLIC
	${IMGUI_DIR}
	${SDL2_INCLUDE_DIRS}
)

target_include_directories(hiruki PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${IMGUI_DIR}
	${SDL2_INCLUDE_DIRS}
)

find_package(OpenMP REQUIRED)

target_link_libraries(hiruki SDL2 SDL2_image imgui OpenMP::OpenMP_CXX)
