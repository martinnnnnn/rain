#pragma once

#include "core/core.h"
#include "math/math.h"


#include <vector>
#include <deque>

namespace rain::engine::ui
{
    struct text_field
    {
        core::uuid id;
        u32 x;
        u32 y;
		u32 width;
		u32 height;
        char buffer[127];
        u32 next_index;
        bool is_focused;
        math::vec4 color_bg;
        math::vec4 color_txt;
        std::string default_text;
        core::signal<std::string const &> on_validate;
    };

    struct text_list
    {
        std::deque<std::string> messages;
        u32 max_count;
        u32 x;
        u32 y;
        math::vec4 color;
    };


    struct ingame_chat
    {
        text_field type_zone;
        text_list text_zone;
    };

    void update(text_field& field);
    void draw(const text_field& field);
    void draw(const text_list& list);

    void add_message(text_list& chat, const std::string& message);
    void add_letter(text_field& field, char letter);
    void remove_letter(text_field& field);
    void clear(text_field& field);
   
}