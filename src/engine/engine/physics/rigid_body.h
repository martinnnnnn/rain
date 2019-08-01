#pragma once


#include <vector>

#include "glm.hpp"
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

        glm::vec3 force;
        glm::vec3 momentum;
        glm::vec3 velocity;

        glm::vec3 torque;
        glm::vec3 angularMomentum;
        glm::vec3 angularVelocity;
        glm::quat spin;
        f32 rotationInertia;
        f32 rotationInertiaInverse;

        bool applyGravity;
        bool infiniteMass;
    };


    void apply_physics(RigidBody& _body, core::transform& _transform, const f32 _deltaTime);

}