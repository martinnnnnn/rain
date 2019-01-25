#pragma once

#include "core/core.h"
#include "math/math.h"

#include "rigid_body.h"
#include "collision.h"
#include "spring.h"

namespace rain::engine
{
    using transform = math::Transform;
    using vec3 = math::vec3;
    using vec4 = math::vec4;

    namespace Physics
    {
        void apply_gravity(RigidBody& _body);
        void apply_spring(const Spring& _spring, const Transform& _transformA, RigidBody& _bodyA, const Transform& _transformB, RigidBody& _bodyB);
        void update(RigidBody& _body, Transform& _transform, float _deltaTime);
    };
}