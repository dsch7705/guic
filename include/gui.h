#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "gui_element.h"

typedef struct GUI_State
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	GUI_Node* base_node;

	GUI_Node* hovered_node;
	GUI_Node* selected_node;

} GUI_State;

GUI_State* gui_create_state(SDL_Window* sdl_window);
void gui_set_active_state(GUI_State* state);
void gui_destroy_state(GUI_State* state);
void gui_update();
void gui_draw();
void gui_handle_sdl_event(SDL_Event* e);