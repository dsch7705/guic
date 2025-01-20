#pragma once
#include <stdlib.h>
#include <stdarg.h>
#include "gui_primatives.h"
#include "gui_font.h"


#define GUI_ELEMENT_TYPE_BASE -1
#define GUI_ELEMENT_TYPE_TEXT -2
#define GUI_ELEMENT_TYPE_PANEL -3
#define GUI_ELEMENT_TYPE_BUTTON -4
#define GUI_ELEMENT_TYPE_TEXT_INPUT -5

#define GUI_FLAG_DEFAULT (0)
#define GUI_FLAG_STATIC (1 << 0)
#define GUI_FLAG_TEXT_CENTERED (1 << 1)
#define GUI_FLAG_HOVERED (1 << 2)

#define GUI_PZERO (GUI_Point) { 0, 0 }

#define GUI_BLACK (GUI_Color) { 0, 0, 0, 255 }
#define GUI_WHITE (GUI_Color) { 255, 255, 255, 255 }
#define GUI_GRAY (GUI_Color) { 128, 128, 128, 255 }
#define GUI_LIGHTGRAY (GUI_Color) { 211, 211, 211, 255 }
#define GUI_DARKLIGHTGRAY (GUI_Color) { 169, 169, 169, 255 }
#define GUI_LIGHTDARKGRAY (GUI_Color) { 87, 87, 87, 255 }
#define GUI_DARKGRAY (GUI_Color) { 45, 45, 45, 255 }
#define GUI_RED (GUI_Color) { 255, 0, 0, 255 }
#define GUI_GREEN (GUI_Color) { 0, 255, 0, 255 }
#define GUI_BLUE (GUI_Color) { 0, 0, 255, 255 }
#define GUI_YELLOW (GUI_Color) { 255, 255, 0, 255 }
#define GUI_CYAN (GUI_Color) { 0, 255, 255, 255 }
#define GUI_MAGENTA (GUI_Color) { 255, 0, 255, 255 }

#define GUI_MAX_TEXT_INPUT_LENGTH 1024

typedef struct GUI_Node
{

	struct GUI_Node* parent;
	struct GUI_Node* child;
	struct GUI_Node* sibling;
	// relative to parent node
	GUI_Point		 position;
	// absolute
	GUI_Point		 position_abs;
	GUI_Size		 size;

	void* element;
	int element_type;

	int flags;

} GUI_Node;

typedef struct GUI_Text
{

	char text[GUI_MAX_TEXT_INPUT_LENGTH];
	int text_len;

	GUI_Font* font;
	GUI_FontStyle* font_style;

	SDL_Texture* _tex;
	unsigned char _redraw;
	GUI_Node* node;

} GUI_Text;
typedef struct GUI_Panel
{

	const char* label;
	GUI_Color color;
	GUI_Node* node;

} GUI_Panel;
typedef struct GUI_TextInput
{

	GUI_Text* text;
	GUI_Node* node;

} GUI_TextInput;
typedef struct GUI_Button
{

	GUI_Text* label;
	void (*action)();

	GUI_Node* node;

} GUI_Button;
typedef struct GUI_ProgressBar
{

	int x;

} GUI_ProgressBar;
typedef struct GUI_ProgressCircle
{

	GUI_PrimativeCircle primative;
	int percent;

} GUI_ProgressCircle;
typedef struct GUI_ProgressRing
{

	GUI_PrimativeRing primative;
	int percent;

} GUI_ProgressRing;

GUI_Node* gui_create_node();
GUI_Panel* gui_create_panel(GUI_Node* parent, const char* label, GUI_Rect rect, GUI_Color color, int flags);
GUI_Text* gui_create_text(GUI_Node* parent, const char* text, GUI_Point pos, GUI_Font* font, GUI_FontStyle* font_style, int flags);
GUI_Button* gui_create_button(GUI_Node* parent, const char* label, GUI_Rect rect, int flags);
GUI_TextInput* gui_create_text_input(GUI_Node* parent, GUI_Rect rect, int flags);

void gui_edit_text(GUI_Text* gui_text, char* new_text, size_t new_text_len);

// Sets position relative to parent
void gui_set_position(GUI_Node* node, GUI_Point new_pos);