#include "text_field.h"

#include "engine/core/context.h"
#include "engine/win32/win32_input.h"
#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/win32/win32_window.h"

namespace rain::engine::ui
{

	void ui::remove_letter(text_field& field)
	{
		if (field.is_focused && field.next_index > 0)
		{
			--field.next_index;
		}
	}

    void ui::clear(text_field& field)
    {
        field.next_index = 0;
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
        RAIN_RENDERER->draw_ui_quad(f32(field.posx), f32(field.posy), f32(field.width), f32(field.height), math::vec3{ 1, 0, 0 });
        if (field.next_index == 0)
        {
            RAIN_RENDERER->draw_text_2d(field.default_text, field.posx, field.posy, 0.5f, math::vec4{ field.color.x, field.color.y, field.color.z, 0.5f });
        }
        else
        {
            RAIN_RENDERER->draw_text_2d(std::string(field.buffer, field.next_index), field.posx, field.posy, 0.5f, field.color);
        }
	}

	void ui::update()
	{
		i32 x_click = RAIN_INPUT->x_from_topleft;
		i32 y_click = (RAIN_WINDOW->get_rect().bottom - RAIN_WINDOW->get_rect().top) - RAIN_INPUT->y_from_topleft;

        if (RAIN_INPUT->mouseclick_left == Input::mouse_state::pressed && RAIN_INPUT->mouseclick_left_time == 0.0)
        {
            if (u32(x_click) > field.posx && u32(x_click) < field.posx + field.width
                && u32(y_click) > field.posy && u32(y_click) < field.posy + field.height)
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
                RAIN_LOG("enter");
                clear(field);
            }
        }
	}



    void ui::init()
    {
        memset(&field, 0, sizeof(text_field));
        field.posx = 100;
        field.posy = 100;
        field.width = 100;
        field.height = 100;
        field.color = math::vec4{ 0.0f, 1.0f, 0.0f, 1.0f };
        field.default_text = "hi !";
    }

}