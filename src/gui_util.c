#include "gui_util.h"


void sdl_translate_verts(int x, int y, SDL_Vertex* src, SDL_Vertex* dst, size_t count)
{
	for (int i = 0; i < count; i++)
	{
		dst[i] = src[i];
		dst[i].position.x = src[i].position.x + x;
		dst[i].position.y = src[i].position.y + y;
	}
}