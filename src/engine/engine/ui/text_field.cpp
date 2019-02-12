#include "text_field.h"

#include "engine/core/context.h"
#include "engine/win32/win32_input.h"
#include "engine/gfx/ogl/ogl_renderer.h"


namespace rain::engine::ui
{

	void ui::remove_letter(text_field& field)
	{
		if (field.is_focused && field.next_index > 0)
		{
			--field.next_index;
		}
	}

	void ui::add_letter(text_field& field, char letter)
	{
		if (field.is_focused && field.next_index < sizeof(field.buffer))
		{
			field.buffer[field.next_index++] = letter;
		}
	}

	void ui::draw()
	{
		//RAIN_RENDERER->draw
	}

	void ui::update()
	{
		i32 x_click = RAIN_INPUT->x_from_topleft;
		i32 y_click = RAIN_INPUT->y_from_topleft;

		if (RAIN_INPUT->mouseclick_left
			&& x_click > field.posx && x_click < field.posx + field.width
			&& y_click > field.posy && y_click < field.posy + field.height)
		{
			field.is_focused = true;
		}
		else
		{
			field.is_focused = false;
		}
	}

	void ui::init()
	{
		memset(&field, 0, sizeof(text_field));
		field.posx = 200;
		field.posy = 200;
		field.width = 50;
		field.height = 50;
	}

}