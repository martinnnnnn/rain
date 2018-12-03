#pragma once

#include <string>
#include <entt.hpp>

#include "core/file.h"

namespace rain
{
    struct World
    {
        std::string name;
        entt::DefaultRegistry registry;
        File file;

        void init(const std::string& _path);

        void update_physics(const float _deltaTime);
        void render(const float _alpha);
    };

}

#define RAIN_NEW_ID rain::Application::get().id_generator->get_new_unique_id()