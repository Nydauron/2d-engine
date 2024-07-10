cmake_minimum_required(VERSION 3.21)

project(2DRenderer)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # helps out clangd LSP

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
  battery-embed
  GIT_REPOSITORY https://github.com/batterycenter/embed.git
  GIT_TAG        v1.2.16
)
FetchContent_MakeAvailable(battery-embed)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shaders/)
add_executable(SDL2Test main.cpp glad.c src/shader.cpp)

target_link_libraries(SDL2Test PRIVATE ${SDL2_LIBRARIES})
target_link_options(SDL2Test PUBLIC -lGL)

b_embed(SDL2Test shaders/fragment_shader.glsl)
b_embed(SDL2Test shaders/vertex_shader.glsl)
