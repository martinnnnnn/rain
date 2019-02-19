#pragma once

#include "core/core.h"
#include "math/math.h"

#include "engine/network/server.h"

#include <deque>

namespace rain::engine::ui
{
    struct text_list
    {
        std::deque<std::string> messages;
        u32 max_count;
        u32 x;
        u32 y;
        math::vec4 color;

        void add_line(const network::INGAME_CHAT_INC& line);
    };

    void draw(const text_list& list);
}