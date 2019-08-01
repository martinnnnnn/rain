#pragma once

#include "core/core.h"
#include "glm.hpp"

#include "rigid_body.h"
#include "collision.h"
#include "spring.h"

namespace rain::engine
{

    namespace Physics
    {
        void apply_gravity(RigidBody& _body);
        void apply_spring(const Spring& _spring, const core::transform& _transformA, RigidBody& _bodyA, const core::transform& _transformB, RigidBody& _bodyB);
        void update(RigidBody& _body, core::transform& _transform, float _deltaTime);
    };
}