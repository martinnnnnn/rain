#pragma once


#include <vector>

#include "math/math.h"
#include "core/core.h"

namespace rain::engine
{

    struct RigidBody
    {
        RigidBody()
            : mass(1.0f)
            , mass_inverse(1.0f / mass)
            , size(1.0f)

            , force{}
            , momentum{}
            , velocity{}

            , torque{}
            , angularMomentum{}
            , angularVelocity{}
            , spin{}
            , rotationInertia(powf(6.0f * size, 2.0f * mass))
            , rotationInertiaInverse(1.0f / rotationInertia)

            , applyGravity(true)
            , infiniteMass(false)
        {}

        f32 mass;
        f32 mass_inverse;
        f32 size;

        math::vec3 force;
        math::vec3 momentum;
        math::vec3 velocity;

        math::vec3 torque;
        math::vec3 angularMomentum;
        math::vec3 angularVelocity;
        math::quat spin;
        f32 rotationInertia;
        f32 rotationInertiaInverse;

        bool applyGravity;
        bool infiniteMass;
    };


    void apply_physics(RigidBody& _body, math::Transform& _transform, const f32 _deltaTime);

}