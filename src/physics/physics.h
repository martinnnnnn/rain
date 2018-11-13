#pragma once

#include <entt.hpp>

#include "rigid_body.h"
#include "collision.h"

namespace rain
{



    namespace Physics
    {
        void apply_gravity(RigidBody& _body);
        void apply_spring(const Spring& _spring, const Transform& _transform, RigidBody& _body);
        void apply_spring(const SpringRope& _spring, const Transform& _transformA, RigidBody& _bodyA, const Transform& _transformB, RigidBody& _bodyB);

        void update(RigidBody& _body, Transform& _transform, float _deltaTime);
    };
}