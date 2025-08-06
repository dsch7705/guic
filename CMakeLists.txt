cmake_minimum_required(VERSION 3.26)
project(guic)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)
set(USE_ASAN ON)

add_subdirectory(lib/SDL)
add_subdirectory(lib/SDL_ttf)

set(FONTS_DIR ${CMAKE_SOURCE_DIR}/fonts)
add_subdirectory(src)