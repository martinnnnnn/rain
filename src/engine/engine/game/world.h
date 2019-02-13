#pragma once

#include <string>
#include <entt.hpp>

#include "core/core.h"
#include "math/math.h"
#include "engine/data/geometry/mesh.h"
#include "engine/network/client.h"

namespace rain::engine
{

    struct World
    {
        std::string name;
        entt::DefaultRegistry registry;
        core::File file;

        void init(const std::string& _path);

        void update_camera(const float _deltaTime);
        void update_physics(const float _deltaTime);
        void draw(const float _alpha);
        u32 temp_vao;

    };
}

