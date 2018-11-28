#pragma once

#include <string>
#include <entt.hpp>

#include "core/file.h"

namespace rain
{
    struct Archivist;


    struct World
    {
        std::string name;
        entt::DefaultRegistry registry;
        File file;

        void init(const std::string& _path);

        void update_physics(const float _deltaTime);
        void render(const float _alpha);
    };

    void archive(Archivist* p_, World& _world, const var_info& _info);
}

#define RAIN_NEW_ID rain::Application::get().id_generator->get_new_unique_id()