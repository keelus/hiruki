cmake_minimum_required(VERSION 3.10)
project(example2)

add_executable(example2 main.cpp)
target_link_libraries(example2 PRIVATE hiruki)

add_custom_target(update_assets_example2 ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf "$<TARGET_FILE_DIR:example2>/assets"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:example2>/assets"
)
