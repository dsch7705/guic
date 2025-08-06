#pragma once
#include <stdio.h>
#include <SDL3/SDL.h>

#define N_CIRCLE_VERTS 50


typedef SDL_Point GUI_Point;
typedef struct GUI_Size
{

	float w;
	float h;

} GUI_Size;
typedef SDL_FRect GUI_Rect;
typedef SDL_Color GUI_Color;

static int gui_primative_circle_indices[N_CIRCLE_VERTS * 3];
typedef struct GUI_PrimativeCircle
{

	SDL_Vertex vertices[N_CIRCLE_VERTS + 1];
	GUI_Color color;

} GUI_PrimativeCircle;
static int gui_primative_ring_indices[N_CIRCLE_VERTS * 6];
typedef struct GUI_PrimativeRing
{

	SDL_Vertex vertices[N_CIRCLE_VERTS * 2];
	GUI_Color color;

} GUI_PrimativeRing;

GUI_PrimativeCircle gui_create_primative_circle(int radius, GUI_Color color);
GUI_PrimativeRing gui_create_primative_ring(int inner_radius, int outer_radius, GUI_Color color);