#include "text_list.h"

#include "engine/core/context.h"
#include "engine/gfx/ogl/ogl_renderer.h"

namespace rain::engine::ui
{
    void text_list::add_line(const network::INGAME_CHAT_INC& line)
    {
        messages.push_front(line.msg);
        if (messages.size() >= max_count)
        {
            messages.pop_back();
        }
    }

    void draw(const text_list& list)
    {
        for (u32 i = 0; i < list.messages.size(); ++i)
        {
            RAIN_RENDERER->draw_text_2d(list.messages[i], f32(list.x), f32(list.y + i * 10), 0.25f, list.color);
        }
    }
}