cmake_minimum_required(VERSION 3.21)

project(2DRenderer)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # helps out clangd LSP

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_executable(SDL2Test main.cpp glad.c)
target_link_libraries(SDL2Test ${SDL2_LIBRARIES})
target_link_options(SDL2Test PUBLIC -lGL)
