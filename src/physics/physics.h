#pragma once

#include <entt.hpp>

#include "rigid_body.h"
#include "math/transform.h"

namespace rain
{
    class Physics
    {
    public:
        static void init(entt::DefaultRegistry& _registry);
        static void update(entt::DefaultRegistry& _registry, float _deltaTime);
    };
}