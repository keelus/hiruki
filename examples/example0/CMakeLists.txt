cmake_minimum_required(VERSION 3.10)

project(example0)

add_executable(example0 main.cpp)

target_link_libraries(example0 PRIVATE hiruki)

set(IMAGE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tiles.png")

add_custom_command(
    TARGET example0 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${IMAGE_FILE} $<TARGET_FILE_DIR:example0>
)
