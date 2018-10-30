#pragma once

#include <entt.hpp>

#include "rigid_body.h"
#include "math/transform.h"

namespace rain
{
    namespace Physics
    {
        void update(entt::DefaultRegistry _registry, float _deltaTime);
        void init(entt::DefaultRegistry _registry, float _deltaTime);
    };
}