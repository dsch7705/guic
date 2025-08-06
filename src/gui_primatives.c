#include "gui_primatives.h"


#include <math.h>


GUI_PrimativeCircle gui_create_primative_circle(int radius, GUI_Color color)
{
	static int first_call = 1;

	GUI_PrimativeCircle circle;
	circle.vertices[0].position.x = 0;
	circle.vertices[0].position.y = 0;

	for (int i = 0; i < N_CIRCLE_VERTS; i++)
	{
		int v_i = i + 1;
		float theta = (SDL_PI_F * 2 / N_CIRCLE_VERTS) * i;
		circle.vertices[v_i].position.x = cosf(theta) * radius;
		circle.vertices[v_i].position.y = sinf(theta) * radius;

		SDL_FColor col = { color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f };
		circle.vertices[v_i].color = col;

		if (first_call)
		{
			gui_primative_circle_indices[i * 3] = 0;
			gui_primative_circle_indices[i * 3] = v_i;
			gui_primative_circle_indices[i * 3] = v_i % N_CIRCLE_VERTS + 1;
		}
	}

	first_call = 0;
	return circle;
}
GUI_PrimativeRing gui_create_primative_ring(int inner_radius, int outer_radius, GUI_Color color)
{
	static int first_call = 1;

	GUI_PrimativeRing ring;

	for (int i = 0; i < N_CIRCLE_VERTS; i++)
	{
		float theta = (2 * SDL_PI_F / N_CIRCLE_VERTS) * i;
		float v_cos = cos(theta);
		float v_sin = sin(theta);

		ring.vertices[i * 2].position.x = v_cos * outer_radius;
		ring.vertices[i * 2 + 1].position.x = v_cos * inner_radius;
		ring.vertices[i * 2].position.y = v_sin * outer_radius;
		ring.vertices[i * 2 + 1].position.y = v_sin * inner_radius;

		SDL_FColor col = { color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f };
		ring.vertices[i * 2].color = col;
		ring.vertices[i * 2 + 1].color = col;

		if (first_call)
		{
			unsigned int v_i = i * 2;
			gui_primative_ring_indices[i * 6] = v_i;
			gui_primative_ring_indices[i * 6 + 1] = (v_i + 1) % (N_CIRCLE_VERTS * 2);
			gui_primative_ring_indices[i * 6 + 2] = (v_i + 2) % (N_CIRCLE_VERTS * 2);
			gui_primative_ring_indices[i * 6 + 3] = (v_i + 2) % (N_CIRCLE_VERTS * 2);
			gui_primative_ring_indices[i * 6 + 4] = (v_i + 1) % (N_CIRCLE_VERTS * 2);
			gui_primative_ring_indices[i * 6 + 5] = (v_i + 3) % (N_CIRCLE_VERTS * 2);
		}
	}

	first_call = 0;
	return ring;
}