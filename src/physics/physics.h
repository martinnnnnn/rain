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
        static void apply_gravity(RigidBody& _body);
        static void apply_spring(const Spring& _spring, const Transform& _transform, RigidBody& _body);

        static void update(RigidBody& _body, Transform& _transform, float _deltaTime);
    };
}