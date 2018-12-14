#pragma once

#include <string>
#include <entt.hpp>

#include "core/file.h"
#include "data/geometry/mesh.h"

namespace rain
{
    struct Boid
    {
        glm::vec3 position;

    };

    struct World
    {
        std::string name;
        entt::DefaultRegistry registry;
        File file;

        void init(const std::string& _path);

        void update_physics(const float _deltaTime);
        void render(const float _alpha);

        //Mesh mesh;
        //std::vector<glm::vec3> positions;

    };

}

