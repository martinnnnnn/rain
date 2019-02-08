#pragma once

#include "core/core.h"
#include "math/math.h"
#include "engine/win32/win32_input.h"

#include <vector>

namespace rain::engine::ui
{
    struct text_field
    {
        u32 posx;
        u32 posy;
        u32 width;
        char buffer[127];
        u32 next_index;
        bool is_focused;
        math::vec4 color;
    };


    struct ui
    {
        std::vector<text_field> text_fields;

        void update()
        {
            if (RAIN_INPUT->mouseclick_left)
            {

            }
        }

        void add_letter(text_field& field, char letter)
        {
            if (field.is_focused && field.next_index < sizeof(field.buffer))
            {
                field.buffer[field.next_index++] = letter;
            }
        }

        void remove_letter(text_field& field)
        {
            if (field.is_focused && field.next_index > 0)
            {
                --field.next_index;
            }
        }
    };
}