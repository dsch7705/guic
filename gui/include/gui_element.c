#include "gui_element.h"


// Returns pointer to a base node
GUI_Node* gui_create_node()
{
	GUI_Node* node = (GUI_Node*)malloc(sizeof(GUI_Node));
	if (!node) { return NULL; }

	node->parent = NULL;
	node->sibling = NULL;
	node->child = NULL;
	node->element = NULL;
	node->element_type = GUI_ELEMENT_TYPE_BASE;
	node->position = (GUI_Point){ 0, 0 };
	node->position_a = (GUI_Point){ 0, 0 };
	node->size = (GUI_Size){ 0, 0 };
	node->flags = GUI_FLAG_DEFAULT;

	return node;
}
// Set the child node or append to the last sibling node if it isn't the first child
GUI_Node* gui_create_node_child(GUI_Node* parent)
{
	GUI_Node* node = gui_create_node();
	node->parent = parent;
	if (parent->child == NULL)
	{
		parent->child = node;
		return node;
	}

	// Go to last sibling node
	GUI_Node* current = parent->child;
	while (current->sibling != NULL)	
		current = current->sibling;

	current->sibling = node;
	return node;
}
// Append to the last sibling node
GUI_Node* gui_create_node_sibling(GUI_Node* child)
{
	GUI_Node* node = gui_create_node();
	node->parent = child->parent;

	GUI_Node* current = child;
	while (current->sibling != NULL)
		current = current->sibling;

	current->sibling = node;
	return node;
}

// Calculate the absolute position of a node
void gui_calc_position_a(GUI_Node* node)
{
	node->position_a = node->position;
	GUI_Node* current = node;
	while (current->parent != NULL)
	{
		node->position_a.x += current->parent->position.x;
		node->position_a.y += current->parent->position.y;

		current = current->parent;
	}
}
GUI_Panel* gui_create_panel(GUI_Node* parent, const char* label, GUI_Rect rect, GUI_Color color, int flags)
{
	GUI_Panel* gui_panel = (GUI_Panel*)malloc(sizeof(GUI_Panel));
	if (!gui_panel) { return NULL; }

	gui_panel->node = gui_create_node_child(parent);
	gui_panel->node->element = gui_panel;
	gui_panel->node->element_type = GUI_ELEMENT_TYPE_PANEL;
	gui_panel->node->flags = flags;

	gui_panel->node->position = (GUI_Point){ rect.x, rect.y };
	gui_calc_position_a(gui_panel->node);
	gui_panel->node->size = (GUI_Size){ rect.w, rect.h };
	gui_panel->color = color;

	return gui_panel;
}
GUI_Text* gui_create_text(GUI_Node* parent, const char* text, GUI_Point pos, GUI_Font* font, GUI_FontStyle* font_style, int flags)
{
	GUI_Text* gui_text = (GUI_Text*)malloc(sizeof(GUI_Text));
	if (!gui_text) { return NULL; }

	gui_text->node = gui_create_node_child(parent);
	gui_text->node->element = gui_text;
	gui_text->node->element_type = GUI_ELEMENT_TYPE_TEXT;
	gui_text->node->flags = flags;

	gui_text->node->position = pos;
	gui_calc_position_a(gui_text->node);
	gui_text->text_len = strlen(text);
	strcpy_s(gui_text->text, gui_text->text_len + 1, text);

	gui_text->font = font;
	gui_text->font_style = font_style;
	gui_text->_tex = NULL;

	return gui_text;
}
GUI_Button* gui_create_button(GUI_Node* parent, const char* label, GUI_Rect rect, int flags)
{
	GUI_Button* gui_button = (GUI_Button*)malloc(sizeof(GUI_Button));
	if (!gui_button) { return NULL; }

	gui_button->node = gui_create_node_child(parent);
	gui_button->node->element = gui_button;
	gui_button->node->element_type = GUI_ELEMENT_TYPE_BUTTON;
	gui_button->node->flags = flags;

	gui_button->node->position = (GUI_Point){ rect.x, rect.y };
	gui_calc_position_a(gui_button->node);
	gui_button->node->size = (GUI_Size){ rect.w, rect.h };

	
	gui_button->label = gui_create_text(gui_button->node, label, GUI_PZERO, GUI_Default_Font, &GUI_Default_FontStyle, 
						GUI_FLAG_STATIC | GUI_FLAG_TEXT_CENTERED);
	//gui_button->node->size.w = gui_button->label->node->size.w * 2;
	//gui_button->node->size.h = gui_button->label->node->size.h * 1.5;

	return gui_button;
}

void gui_set_position(GUI_Node* node, GUI_Point new_pos)
{
	node->position = new_pos;
	gui_calc_position_a(node);
}