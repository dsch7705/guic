#pragma once
#include <SDL3/SDL.h>


void sdl_translate_verts(int x, int y, SDL_Vertex* src, SDL_Vertex* dst, size_t count);