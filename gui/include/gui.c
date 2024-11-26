#include "gui.h"


GUI_State* gui_create_state(SDL_Renderer* sdl_renderer)
{
	GUI_State* state = (GUI_State*)malloc(sizeof(GUI_State));
	if (!state) { return NULL; }

	state->renderer = sdl_renderer;
	state->base_node = gui_create_node();
	return state;
}

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

int gui_is_mouse_hovering(GUI_Node* node)
{
	if (GUI_MouseState.x > node->position_a.x && GUI_MouseState.y > node->position_a.y &&
		GUI_MouseState.x < node->position_a.x + node->size.w && GUI_MouseState.y < node->position_a.y + node->size.h)
		return 1;
	return 0;
}
void gui_get_hovered_node(GUI_State* state)
{
	GUI_Node* node_ptr = state->base_node->child;
	while (node_ptr != NULL)
	{
		if (gui_is_mouse_hovering(node_ptr))
		{
			if (node_ptr->flags & GUI_FLAG_STATIC != 0)
			{
				state->hovered_node = node_ptr->parent;
				break;
			}

			if (node_ptr->child == NULL)
			{
				state->hovered_node = node_ptr;
				break;
			}

			node_ptr = node_ptr->child;
			continue;
		}
		if (node_ptr->sibling == NULL)
		{
			state->hovered_node = node_ptr->parent;
			break;
		}

		node_ptr = node_ptr->sibling;
	}

	if (state->hovered_node)
		state->hovered_node->flags |= GUI_FLAG_HOVERED;
}

void gui_handle_sdl_event(SDL_Event* e)
{
	switch (e->type)
	{
	case SDL_MOUSEMOTION:
		GUI_MouseState.x = e->motion.x;
		GUI_MouseState.y = e->motion.y;
		break;
	case SDL_MOUSEBUTTONDOWN:
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
	case SDL_MOUSEBUTTONUP:
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
	SDL_RenderDrawRect(renderer, rect);
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
	GUI_Rect r = { gui_panel->node->position_a.x, gui_panel->node->position_a.y,
					gui_panel->node->size.w, gui_panel->node->size.h };
	gui_fill_rect(renderer, &r, gui_panel->color);
	
}
void gui_draw_text(SDL_Renderer* renderer, GUI_Text* gui_text)
{
	if (gui_text->_tex == NULL)
	{
		TTF_SetFontSize(gui_text->font->ttf_font, gui_text->font_style->pt_size);
		TTF_SetFontStyle(gui_text->font->ttf_font, gui_text->font_style->style);
		SDL_Surface* sur = TTF_RenderText_Blended_Wrapped(gui_text->font->ttf_font, gui_text->text, 
							gui_text->font_style->color, gui_text->node->parent->size.w);
		gui_text->_tex = SDL_CreateTextureFromSurface(renderer, sur);
		SDL_QueryTexture(gui_text->_tex, NULL, NULL, &gui_text->node->size.w, &gui_text->node->size.h);

		if (gui_text->node->flags & GUI_FLAG_TEXT_CENTERED)
		{
			GUI_Point centered;
			centered.x = (gui_text->node->parent->size.w - gui_text->node->size.w) / 2;
			centered.y = (gui_text->node->parent->size.h - gui_text->node->size.h) / 2;

			gui_set_position(gui_text->node, centered);
		}

		SDL_FreeSurface(sur);
	}

	SDL_Rect r = { gui_text->node->position_a.x, gui_text->node->position_a.y, 
					gui_text->node->size.w, gui_text->node->size.h };

	SDL_RenderCopy(renderer, gui_text->_tex, NULL, &r);
}
void gui_draw_button(SDL_Renderer* renderer, GUI_Button* gui_button, int hovered)
{
	SDL_Rect r = { gui_button->node->position_a.x, gui_button->node->position_a.y,
					gui_button->node->size.w, gui_button->node->size.h };

	GUI_Color c = (gui_button->node->flags & GUI_FLAG_HOVERED) ? GUI_DARKLIGHTGRAY : GUI_LIGHTGRAY;
	gui_fill_rect(renderer, &r, c);
}

void gui_update_branch(GUI_State* state, GUI_Node* root)
{
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
			gui_draw_text(state->renderer, (GUI_Text*)current->element);
			break;
		case GUI_ELEMENT_TYPE_PANEL:
			gui_draw_panel(state->renderer, (GUI_Panel*)current->element);
			break;
		case GUI_ELEMENT_TYPE_BUTTON:
			gui_draw_button(state->renderer, (GUI_Button*)current->element, 0);
			break;
		default:
			break;
		}

		// Reset hovered flag
		current->flags &= ~GUI_FLAG_HOVERED;

		if (current->child != NULL)
			gui_update_branch(state, current->child);

		current = current->sibling;

	} while (current != NULL);
}

void gui_update(GUI_State* state)
{
	//
	if (GUI_MouseState.left == DOWN)
	{
		if (state->hovered_node->element_type == GUI_ELEMENT_TYPE_BUTTON)
		{
			((GUI_Button*)state->hovered_node->element)->action();
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
void gui_draw_state(GUI_State* state)
{
	gui_get_hovered_node(state);
	gui_update_branch(state, state->base_node);
	// Debug lines
	if (state->hovered_node && 0)
	{
		SDL_Rect r = { state->hovered_node->position_a.x, state->hovered_node->position_a.y,
						state->hovered_node->size.w, state->hovered_node->size.h };
		SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(state->renderer, &r);
		//printf("%p\n", state->hovered_node);
	}
}
//