#pragma once

#include <entt.hpp>

#include "rigid_body.h"
#include "collision.h"

namespace rain
{
    class Physics
    {
    public:
        // TODO(martin) : change these interfaces to receive components directly
        static void init(entt::DefaultRegistry& _registry);
        static void update(entt::DefaultRegistry& _registry, float _deltaTime);
    };
}