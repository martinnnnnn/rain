#pragma once

#include <vector>
#include <deque>
#include "glm.hpp"

#include "core/core.h"
#include "engine/network/server.h"


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
        glm::vec4 color_bg;
        glm::vec4 color_txt;
        std::string default_text;
        core::signal<std::string const &> on_validate;
    };

    void update(text_field& field);
    void draw(const text_field& field);
    void add_letter(text_field& field, char letter);
    void remove_letter(text_field& field);
    void clear(text_field& field);

}