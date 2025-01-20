#include <stdio.h>
#include <SDL/SDL.h>
#include "gui.h"

#define ASPECT_RATIO (16. / 9.)			// x / y

#define WINDOW_W 800.
#define WINDOW_H (WINDOW_W / ASPECT_RATIO)

GUI_Panel* panel;
static void set_panel_color_red()
{
	panel->color = GUI_RED;
}
static void set_panel_color_green()
{
	panel->color = GUI_GREEN;
}
static void set_panel_color_blue()
{
	panel->color = GUI_BLUE;
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return -1;
	if (TTF_Init() != 0)
		return -1;
	
	SDL_Window* window = SDL_CreateWindow("GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// GUI

	GUI_State* gui = gui_create_state(renderer);
	gui_set_active_state(gui);
	printf("Loaded %d fonts\n", gui_load_fonts());

	panel = gui_create_panel(gui->base_node, "Test", (GUI_Rect) { 25, 25, WINDOW_W - 50, WINDOW_H - 50 }, GUI_WHITE, 0);
	gui_create_text(panel->node, "The quick brown fox jumps over the lazy dog.", GUI_PZERO, GUI_Default_Font, &GUI_Default_FontStyle, 0);

	GUI_Button* btnR = gui_create_button(panel->node, "Red", (GUI_Rect) { 0, 35, 100, 30 }, 0);
	GUI_Button* btnG = gui_create_button(panel->node, "Green", (GUI_Rect) { 0, 65, 100, 30 }, 0);
	GUI_Button* btnB = gui_create_button(panel->node, "Blue", (GUI_Rect) { 0, 95, 100, 30 }, 0);
	btnR->action = set_panel_color_red;
	btnG->action = set_panel_color_green;
	btnB->action = set_panel_color_blue;

	GUI_TextInput* txtInput = gui_create_text_input(panel->node, (GUI_Rect) { 0, 125, 600, 200 }, 0);

	//
	unsigned char running = 1;
	while (running)
	{
		// Background
		SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
		SDL_RenderClear(renderer);

		gui_draw();
		gui_update();

		SDL_RenderPresent(renderer);

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			gui_handle_sdl_event(&e);
			switch (e.type)
			{
			case SDL_QUIT:
				running = 0;
				break;
			default:
				break;
			}
		}
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}