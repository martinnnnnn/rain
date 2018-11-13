﻿#pragma once


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

            //, position(glm::vec3(0.0f, 0.0f, 0.0f))
            , force(glm::vec3(0.0f, 0.0f, 0.0f))
            , momentum(glm::vec3(0.0f, 0.0f, 0.0f))
            , velocity(glm::vec3(0.0f, 0.0f, 0.0f))

            //, orientation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)))
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

        //glm::vec3 position;
        glm::vec3 force;
        glm::vec3 momentum;
        glm::vec3 velocity;

        //glm::quat orientation;
        glm::vec3 torque;
        glm::vec3 angularMomentum;
        glm::vec3 angularVelocity;
        glm::quat spin;
        f32 rotationInertia;
        f32 rotationInertiaInverse;

        bool applyGravity;
        bool infiniteMass;
    };

    struct Spring
    {
        Spring()
            : entity(0)
            , k(15.0f)
            , b(0.1f)
        {}
        u32 entity;
        f32 k;
        f32 b;
    };
    
    struct SpringRope
    {
        SpringRope()
            : entityA(0)
            , entityB(0)
            , distance(0.0f)
            , k(15.0f)
            , b(0.1f)
        {}

        u32 entityA;
        u32 entityB;
        float distance;
        f32 k;
        f32 b;
    };


    void apply_physics(RigidBody& _body, Transform& _transform, const f32 _deltaTime);
    void apply_physics(RigidBody& _body, Transform& _transform, f32 _deltaTime, const std::vector<glm::vec3>& _forces, const glm::vec3& _torque);

}