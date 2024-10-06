add_library(hiruki STATIC
	math/vector2.cpp
	math/vector3.cpp
	math/vector4.cpp

	graphics/texCoord.cpp

	engine.cpp
)
target_include_directories(hiruki PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
