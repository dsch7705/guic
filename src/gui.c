#include "gui.h"
#include "gui_element.h"


enum GUI_MouseButtonState
{
	OFF,
	UP,
	DOWN,
	HELD
};
struct
{

	int x, y;
	enum GUI_MouseButtonState left, right, middle;

} GUI_MouseState;
GUI_State* active_state = NULL;
GUI_State* gui_create_state(SDL_Window* sdl_window)
{
	GUI_State* state = (GUI_State*)malloc(sizeof(GUI_State));
	if (!state) { return NULL; }

	state->window = sdl_window;
	state->renderer = SDL_CreateRenderer(sdl_window, NULL);
	SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_BLEND);

	state->base_node = gui_create_node();
	state->hovered_node = state->base_node;
	state->selected_node = state->base_node;

	return state;
}
void gui_destroy_state(GUI_State *state)
{
	SDL_DestroyRenderer(state->renderer);
}
void gui_set_active_state(GUI_State* state)
{
	active_state = state;
}
GUI_State* gui_get_active_state()
{
	return active_state;
}

int gui_is_mouse_hovering(GUI_Node* node)
{
	if (GUI_MouseState.x > node->position_abs.x && GUI_MouseState.y > node->position_abs.y &&
		GUI_MouseState.x < node->position_abs.x + node->size.w && GUI_MouseState.y < node->position_abs.y + node->size.h)
		return 1;
	return 0;
}
void gui_get_hovered_node()
{
	static GUI_Node* last_hovered_node = NULL;
	GUI_Node* node_ptr = active_state->base_node->child;
	while (node_ptr != NULL)
	{
		if (gui_is_mouse_hovering(node_ptr))
		{
			if ((node_ptr->flags & GUI_FLAG_STATIC) != 0)
			{
				active_state->hovered_node = node_ptr->parent;
				break;
			}

			if (node_ptr->child == NULL)
			{
				active_state->hovered_node = node_ptr;
				break;
			}

			node_ptr = node_ptr->child;
			continue;
		}
		if (node_ptr->sibling == NULL)
		{
			active_state->hovered_node = node_ptr->parent;
			break;
		}

		node_ptr = node_ptr->sibling;
	}

	if (active_state->hovered_node)
		active_state->hovered_node->flags |= GUI_FLAG_HOVERED;
	if (last_hovered_node != active_state->hovered_node)
	{
		if (last_hovered_node != NULL)
			last_hovered_node->flags &= ~GUI_FLAG_HOVERED;
		
		last_hovered_node = active_state->hovered_node;
	}
}

void gui_handle_sdl_event(SDL_Event* e)
{
	switch (e->type)
	{
	case SDL_EVENT_MOUSE_MOTION:
		GUI_MouseState.x = e->motion.x;
		GUI_MouseState.y = e->motion.y;
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		switch (e->button.button)
		{
		case SDL_BUTTON_LEFT:
			GUI_MouseState.left = DOWN;
			break;
		case SDL_BUTTON_RIGHT:
			GUI_MouseState.right = DOWN;
			break;
		case SDL_BUTTON_MIDDLE:
			GUI_MouseState.middle = DOWN;
			break;
		default:
			break;
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		switch (e->button.button)
		{
		case SDL_BUTTON_LEFT:
			GUI_MouseState.left = UP;
			break;
		case SDL_BUTTON_RIGHT:
			GUI_MouseState.right = UP;
			break;
		case SDL_BUTTON_MIDDLE:
			GUI_MouseState.middle = UP;
			break;
		default:
			break;
		}
		break;

	case SDL_EVENT_TEXT_INPUT:
		if (!active_state || active_state->selected_node->element_type != GUI_ELEMENT_TYPE_TEXT_INPUT)
			break;

	{
		GUI_Text* gui_text = (GUI_Text*)active_state->selected_node->child->element;
		gui_edit_text(gui_text, e->text.text, strlen(e->text.text));
		break;
	}
	case SDL_EVENT_KEY_DOWN:
		if (active_state && active_state->selected_node->element_type == GUI_ELEMENT_TYPE_TEXT_INPUT)
		{
			switch (e->key.key)
			{
			case SDLK_BACKSPACE:
			{
				GUI_Text* gui_text = (GUI_Text*)active_state->selected_node->child->element;
				gui_edit_text(gui_text, "BK", 2);
				break;
			}
			default:
				break;
			}
		}
		else
		{
			break;
		}
	default:
		break;
	}
}

// DRAWING
void gui_draw_rect(SDL_Renderer* renderer, GUI_Rect* rect, GUI_Color col)
{
	SDL_Color tmp_col;
	SDL_GetRenderDrawColor(renderer, &tmp_col.r, &tmp_col.g, &tmp_col.b, &tmp_col.a);
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
	SDL_RenderRect(renderer, rect);
	SDL_SetRenderDrawColor(renderer, tmp_col.r, tmp_col.g, tmp_col.b, tmp_col.a);
}
void gui_fill_rect(SDL_Renderer* renderer, GUI_Rect* rect, GUI_Color col)
{
	SDL_Color tmp_col;
	SDL_GetRenderDrawColor(renderer, &tmp_col.r, &tmp_col.g, &tmp_col.b, &tmp_col.a);
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
	SDL_RenderFillRect(renderer, rect);
	SDL_SetRenderDrawColor(renderer, tmp_col.r, tmp_col.g, tmp_col.b, tmp_col.a);
}

void gui_draw_panel(SDL_Renderer* renderer, GUI_Panel* gui_panel)
{
	// Draw rect
	GUI_Rect r = { gui_panel->node->position_abs.x, gui_panel->node->position_abs.y,
					gui_panel->node->size.w, gui_panel->node->size.h };
	gui_fill_rect(renderer, &r, gui_panel->color);
	
}
void gui_draw_text(SDL_Renderer* renderer, GUI_Text* gui_text, unsigned char redraw)
{
	if (gui_text->_tex == NULL || redraw)
	{
		if (gui_text->_tex)
			SDL_DestroyTexture(gui_text->_tex);

		TTF_SetFontSize(gui_text->font->ttf_font, gui_text->font_style->pt_size);
		TTF_SetFontStyle(gui_text->font->ttf_font, gui_text->font_style->style);
		SDL_Surface* sur = TTF_RenderText_Blended_Wrapped(gui_text->font->ttf_font, gui_text->text, strlen(gui_text->text), 
							gui_text->font_style->color, gui_text->node->parent->size.w);
		gui_text->_tex = SDL_CreateTextureFromSurface(renderer, sur);

		SDL_GetTextureSize(gui_text->_tex, &gui_text->node->size.w, &gui_text->node->size.h);

		if (gui_text->node->flags & GUI_FLAG_TEXT_CENTERED)
		{
			GUI_Point centered;
			centered.x = (gui_text->node->parent->size.w - gui_text->node->size.w) / 2;
			centered.y = (gui_text->node->parent->size.h - gui_text->node->size.h) / 2;

			gui_set_position(gui_text->node, centered);
		}

		gui_text->_redraw = 0;
		SDL_DestroySurface(sur);
	}

	SDL_FRect r = { gui_text->node->position_abs.x, gui_text->node->position_abs.y, 
					gui_text->node->size.w, gui_text->node->size.h };

	SDL_RenderTexture(renderer, gui_text->_tex, NULL, &r);
}
void gui_draw_button(SDL_Renderer* renderer, GUI_Button* gui_button, int hovered)
{
	SDL_FRect r = { gui_button->node->position_abs.x, gui_button->node->position_abs.y,
					gui_button->node->size.w, gui_button->node->size.h };

	printf("%d\n", gui_button->node->flags);
	GUI_Color c = (gui_button->node->flags & GUI_FLAG_HOVERED) ? GUI_DARKLIGHTGRAY : GUI_LIGHTGRAY;
	gui_fill_rect(renderer, &r, c);
}
void gui_draw_text_input(SDL_Renderer* renderer, GUI_TextInput* gui_text_input)
{
	SDL_FRect r = { gui_text_input->node->position_abs.x, gui_text_input->node->position_abs.y,
					gui_text_input->node->size.w, gui_text_input->node->size.h };
	gui_draw_rect(renderer, &r, GUI_BLACK);

}

void gui_update_branch(GUI_Node* root)
{
	if (!active_state)
	{
		printf("No state is active!");
		return;
	}
	if (!root)
	{
		printf("Root node is null!");
		return;
	}
	GUI_Node* current = root;
	do
	{
		// Dispatch element draw funcs
		switch (current->element_type)
		{
		case GUI_ELEMENT_TYPE_TEXT:
			gui_draw_text(active_state->renderer, (GUI_Text*)current->element, ((GUI_Text*)current->element)->_redraw);
			break;
		case GUI_ELEMENT_TYPE_PANEL:
			gui_draw_panel(active_state->renderer, (GUI_Panel*)current->element);
			break;
		case GUI_ELEMENT_TYPE_BUTTON:
			gui_draw_button(active_state->renderer, (GUI_Button*)current->element, 1);
			break;
		case GUI_ELEMENT_TYPE_TEXT_INPUT:
			gui_draw_text_input(active_state->renderer, (GUI_TextInput*)current->element);
			break;
		default:
			break;
		}

		// Reset hovered flag
		//current->flags &= ~GUI_FLAG_HOVERED;

		if (current->child != NULL)
			gui_update_branch(current->child);

		current = current->sibling;

	} while (current != NULL);
}

void gui_update()
{
	if (!active_state)
	{
		printf("No state is active!");
		return;
	}
	//
	if (GUI_MouseState.left == DOWN)
	{
		active_state->selected_node = active_state->hovered_node;

		switch (active_state->hovered_node->element_type)
		{
		case GUI_ELEMENT_TYPE_BUTTON:
			((GUI_Button*)active_state->hovered_node->element)->action();
			break;
		default:
			break;
		}

		switch (active_state->selected_node->element_type)
		{
		case GUI_ELEMENT_TYPE_TEXT_INPUT:
			//SDL_StartTextInput(active_state->window);
			break;
		default:
			//SDL_StopTextInput(active_state->window);
			break;
		}
	}

	// Determine mouse button state after update
	switch (GUI_MouseState.left)
	{
	case DOWN:
		GUI_MouseState.left = HELD;
		break;
	case UP:
		GUI_MouseState.left = OFF;
		break;
	case HELD:
		GUI_MouseState.left = HELD;
		break;
	default:
		GUI_MouseState.left = OFF;
		break;
	}
}
void gui_draw()
{
	gui_get_hovered_node();
	gui_update_branch(active_state->base_node);
	// Debug lines
	if (active_state->hovered_node && 0)
	{
		SDL_FRect r = { active_state->hovered_node->position_abs.x, active_state->hovered_node->position_abs.y,
						active_state->hovered_node->size.w, active_state->hovered_node->size.h };
		SDL_SetRenderDrawColor(active_state->renderer, 255, 0, 0, 255);
		SDL_RenderRect(active_state->renderer, &r);
		//printf("%p\n", state->hovered_node);
	}
}
//