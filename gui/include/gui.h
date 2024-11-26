#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "gui_element.h"

typedef struct GUI_State
{
	
	SDL_Renderer* renderer;
	GUI_Node* base_node;

	GUI_Node* hovered_node;

} GUI_State;

GUI_State* gui_create_state(SDL_Renderer* sdl_renderer);
//void gui_destroy_state(GUI_State* state);
void gui_update(GUI_State* state);
void gui_draw_state(GUI_State* state);
void gui_handle_sdl_event(SDL_Event* e);