#pragma once


#include <vector>

#include "math/math.h"
#include "core/core.h"

namespace rain::engine
{
    using math::Transform;
    using math::quat;
    using math::vec3;

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

        vec3 force;
        vec3 momentum;
        vec3 velocity;

        vec3 torque;
        vec3 angularMomentum;
        vec3 angularVelocity;
        quat spin;
        f32 rotationInertia;
        f32 rotationInertiaInverse;

        bool applyGravity;
        bool infiniteMass;
    };


    void apply_physics(RigidBody& _body, Transform& _transform, const f32 _deltaTime);

}