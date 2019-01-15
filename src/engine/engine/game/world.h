#pragma once

#include <string>
#include <entt.hpp>

#include "core/core.h"
#include "engine/data/geometry/mesh.h"
#include "engine/network/client.h"

namespace rain::engine
{
    using core::math::Transform;

    struct World
    {
        std::string name;
        entt::DefaultRegistry registry;
        core::File file;

        void init(const std::string& _path);

        void update_camera(const float _deltaTime);
        void update_physics(const float _deltaTime);
        void render(const float _alpha);

        //Mesh mesh;
        //std::vector<glm::vec3> positions;

    };
}

