#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "math/transform.h"
#include "core/types.h"

namespace rain
{
    struct RigidBody
    {
        RigidBody()
            : mass(1.0f)
            , mass_inverse(1.0f / mass)
            , size(1.0f)

            , force(glm::vec3(0.0f, 0.0f, 0.0f))
            , momentum(glm::vec3(0.0f, 0.0f, 0.0f))
            , velocity(glm::vec3(0.0f, 0.0f, 0.0f))

            , torque(glm::vec3(0.0f, 0.0f, 0.0f))
            , angularMomentum(glm::vec3(0.0f, 0.0f, 0.0f))
            , angularVelocity(glm::vec3(0.0f, 0.0f, 0.0f))
            , spin(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)))
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


    void apply_physics(RigidBody& _body, Transform& _transform, const f32 _deltaTime);

}