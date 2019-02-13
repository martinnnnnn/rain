#pragma once

#include "core/core.h"
#include "math/math.h"


#include <vector>

namespace rain::engine::ui
{
    struct text_field
    {
        core::uuid id;
        u32 posx;
        u32 posy;
		u32 width;
		u32 height;
        char buffer[127];
        u32 next_index;
        bool is_focused;
        math::vec4 color;
        std::string default_text;
        core::signal<std::string> on_validate_callbacks;
    };

    struct ui
    {
		//std::vector<text_field> text_fields;
		text_field field;
        
		void init();
        void update();
		void draw();

        void add_letter(text_field& field, char letter);
        void remove_letter(text_field& field);
        void clear(text_field& field);
    };
}