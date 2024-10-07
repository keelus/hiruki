add_library(hiruki STATIC
	math/vector2.cpp
	math/vector3.cpp
	math/vector4.cpp

	graphics/texCoord.cpp
	graphics/mesh.cpp
	graphics/renderPipeline.cpp

	engine.cpp
)

target_include_directories(hiruki PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(SDL2 REQUIRED)
target_link_libraries(hiruki SDL2)
