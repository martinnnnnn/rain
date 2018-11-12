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
    };
}