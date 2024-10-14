cmake_minimum_required(VERSION 3.10)
project(example0)

add_executable(example0 main.cpp)
target_link_libraries(example0 PRIVATE hiruki)

add_custom_target(update_assets ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf "$<TARGET_FILE_DIR:example0>/assets"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:example0>/assets"
)
