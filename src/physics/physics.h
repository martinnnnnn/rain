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
        static void apply_spring(const SpringRope& _spring, const Transform& _transformA, RigidBody& _bodyA, const Transform& _transformB, RigidBody& _bodyB);

        static void update(RigidBody& _body, Transform& _transform, float _deltaTime);
    };
}