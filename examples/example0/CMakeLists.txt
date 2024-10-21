cmake_minimum_required(VERSION 3.10)
project(example0)

add_executable(example0 main.cpp)
target_link_libraries(example0 PRIVATE hiruki)
