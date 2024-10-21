cmake_minimum_required(VERSION 3.10)
project(example1)

add_executable(example1 main.cpp)
target_link_libraries(example1 PRIVATE hiruki)

add_custom_target(update_assets_example1 ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf "$<TARGET_FILE_DIR:example1>/assets"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:example1>/assets"
)
