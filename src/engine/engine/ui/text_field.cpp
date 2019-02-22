#include "text_field.h"

#include "engine/core/context.h"
#include "engine/win32/win32_input.h"
#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/win32/win32_window.h"

namespace rain::engine::ui
{



	void remove_letter(text_field& field)
	{
		if (field.is_focused && field.next_index > 0)
		{
			--field.next_index;
		}
	}

    void clear(text_field& field)
    {
        field.next_index = 0;
    }

	void add_letter(text_field& field, char letter)
	{
		if (field.is_focused && field.next_index < sizeof(field.buffer))
		{
			field.buffer[field.next_index++] = letter;
		}
	}

    void draw(const text_field& field)
    {
        RAIN_RENDERER->draw_ui_quad(f32(field.x), f32(field.y), f32(field.width), f32(field.height), field.color_bg);

        assert(field.next_index < NELEMS(field.buffer));
        RAIN_RENDERER->draw_text_2d(std::string(field.buffer, field.next_index), field.x + 2, field.y + 2, 0.25f, field.color_txt);
    }

	void update(text_field& field)
	{
		i32 x_click = RAIN_INPUT->x_from_topleft;
		i32 y_click = (RAIN_WINDOW->get_rect().bottom - RAIN_WINDOW->get_rect().top) - RAIN_INPUT->y_from_topleft;

        if (RAIN_INPUT->mouseclick_left == Input::mouse_state::pressed && RAIN_INPUT->mouseclick_left_time == 0.0)
        {
            if (u32(x_click) > field.x && u32(x_click) < field.x + field.width
                && u32(y_click) > field.y && u32(y_click) < field.y + field.height)
            {
                field.is_focused = true;
            }
            else
            {
                field.is_focused = false;
            }
        }

        if (field.is_focused)
        {
            for (u32 i = 0; i < NELEMS(RAIN_INPUT->alphanum_codes); ++i)
            {
                u32 keycode = RAIN_INPUT->alphanum_codes[i];
                if (RAIN_INPUT->is_key_released(keycode))
                {
                    add_letter(field, RAIN_INPUT->get_char(keycode));
                }
            }
            if (RAIN_INPUT->is_key_released(DIK_BACKSPACE))
            {
                remove_letter(field);
            }
            if (RAIN_INPUT->is_key_released(DIK_RETURN))
            {
                RAIN_LOG("entered !");
                field.on_validate.emit(std::string(field.buffer, field.next_index));
                clear(field);
            }
        }
	}
}