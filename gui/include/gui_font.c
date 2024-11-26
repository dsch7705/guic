#include "gui_font.h"


int is_ttf_file(dirent* de)
{
	if (de->d_namlen < 4) { return 0; }		// Cannot possibly have .ttf extension if name < 4
	if (!strcmp(de->d_name + de->d_namlen - 4, ".ttf")) { return 1; }
	return 0;
}

GUI_Font* font_base = NULL;
int gui_load_fonts()
{
	struct dirent* de;
	DIR* dr = opendir(FONT_DIR);
	int count = 0;

	if (!dr)
	{
		printf("Could not open font directory '%s'", FONT_DIR);
		return count;
	}

	GUI_Font* font_current = font_base;
	while ((de = readdir(dr)) != NULL)
	{
		if (de->d_type != DT_REG || !is_ttf_file(de))
			continue;
		
		char path_to_font[256];
		sprintf_s(path_to_font, 256, "%s/%s", FONT_DIR, de->d_name);

		GUI_Font* font = (GUI_Font*)malloc(sizeof(GUI_Font));							// 
		font->font_name = (char*)malloc(de->d_namlen - 3);								// Absolute
		memcpy_s(font->font_name, de->d_namlen - 3, de->d_name, de->d_namlen - 4);		// Fuckery
		font->font_name[de->d_namlen - 4] = '\0';										//

		font->font_next = NULL;
		font->ttf_font = TTF_OpenFont(path_to_font, 48);

		if (!font->ttf_font)
		{
			printf("Error loading font file '%s': %s\n", de->d_name, TTF_GetError());
			continue;
		}
		if (font_base == NULL)
		{
			font_base = font;
			font_current = font;
		}
		else
		{
			font_current->font_next = font;
			font_current = font_current->font_next;
		}

		count++;
	}

	GUI_Default_Font = font_base;
	GUI_Default_FontStyle.style = TTF_STYLE_NORMAL;
	GUI_Default_FontStyle.pt_size = 24;
	GUI_Default_FontStyle.color = (GUI_Color){ 0, 0, 0, 255 };

	return count;
}
int gui_unload_fonts()
{
	GUI_Font* current = font_base;
	GUI_Font* tmp;
	size_t count = 0;
	while (current != NULL)
	{
		TTF_CloseFont(current->ttf_font);
		tmp = current;
		current = current->font_next;
		free(tmp);

		count++;
	}

	GUI_Default_Font = NULL;
	return count;
}
GUI_Font* gui_get_font(const char* font_name)
{
	if (font_base == NULL) { return font_base; }

	GUI_Font* current = font_base;
	while (current != NULL)
	{
		if (!strcmp(current->font_name, font_name))
			return current;

		current = current->font_next;
	}
	
	return NULL;
}

unsigned char _r = 0;
unsigned char _g = 0;
unsigned char _b = 0;
unsigned char _a = 255;
unsigned char _pt_size = 24;

void gui_set_font_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}
void gui_get_font_color(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a)
{
	*r = _r;
	*g = _g;
	*b = _b;
	*a = _a;
}
void gui_set_font_size(unsigned char pt_size)
{
	_pt_size = pt_size;
}
unsigned char gui_get_font_size()
{
	return _pt_size;
}
