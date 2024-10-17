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
	ALAGARD_RAW.c
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
target_include_directories(hiruki PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${SDL2_INCLUDE_DIRS}
)

find_package(OpenMP REQUIRED)

target_link_libraries(hiruki SDL2 SDL2_image SDL2_ttf OpenMP::OpenMP_CXX)
