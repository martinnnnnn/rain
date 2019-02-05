#pragma once

#include "core/core.h"
#include "math/math.h"

#include "rigid_body.h"
#include "collision.h"
#include "spring.h"

namespace rain::engine
{

    namespace Physics
    {
        void apply_gravity(RigidBody& _body);
        void apply_spring(const Spring& _spring, const math::transform& _transformA, RigidBody& _bodyA, const math::transform& _transformB, RigidBody& _bodyB);
        void update(RigidBody& _body, math::transform& _transform, float _deltaTime);
    };
}