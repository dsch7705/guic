#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "SDL/SDL_ttf.h"

#define FONT_DIR "fonts"


typedef struct GUI_Font
{

	TTF_Font* ttf_font;
	char* font_name;
	struct GUI_Font* font_next;

} GUI_Font;
typedef SDL_Color GUI_Color;
typedef struct GUI_FontStyle
{

	GUI_Color color;
	int		  pt_size;
	int		  style;

} GUI_FontStyle;

GUI_Font* GUI_Default_Font;
GUI_FontStyle GUI_Default_FontStyle;

// Returns number of fonts loaded
int gui_load_fonts();
int gui_unload_fonts();
GUI_Font* gui_get_font(const char* font_name);
