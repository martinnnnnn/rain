#pragma once

#include "core/core.h"
#include "actor.h"

namespace rain::engine
{
    struct scene_graph
    {
        std::vector<actor*> actors;

        actor* create()
        {
            auto new_actor = new actor();
            actors.push_back(new_actor);
            return new_actor;
        }

        template<typename... T>
        void get_view(std::vector<actor*>& view, bool clear = false)
        {
            if (clear) view.clear();

            for (u32 i = 0; i < actors.size(); ++i)
            {
                if (actors[i]->components.has<T...>())
                {
                    view.push_back(actors[i]);
                }
            }
        }
    };
}

