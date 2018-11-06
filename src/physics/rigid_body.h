#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "core/types.h"

namespace rain
{
    struct RigidBody
    {
        RigidBody()
            : orientation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)))
            , angularMomentum(glm::vec3(0.0f, 0.0f, 0.0f))
            , angularVelocity(glm::vec3(0.0f, 0.0f, 0.0f))
            , torque(glm::vec3(0.0f, 0.0f, 0.0f))
            , spin(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)))
            , mass(1.0f)
            , size(1.0f)
            , mass_inverse(1.0f / mass)
            , rotationInertia(powf(6.0f * size, 2.0f * mass))
            , rotationInertiaInverse(1.0f / rotationInertia)
            , position(glm::vec3(0.0f, 0.0f, 0.0f))
            , momentum(glm::vec3(0.0f, 0.0f, 0.0f))
            , velocity(glm::vec3(0.0f, 0.0f, 0.0f))
            , force(glm::vec3(0.0f, 0.0f, 0.0f))
        {}

        float mass;
        float mass_inverse;
        float size;

        glm::vec3 position;
        glm::vec3 force;
        glm::vec3 momentum;
        glm::vec3 velocity;

        glm::quat orientation;
        glm::vec3 torque;
        glm::vec3 angularMomentum;
        glm::vec3 angularVelocity;
        glm::quat spin;
        float rotationInertia;
        float rotationInertiaInverse;
    };


    struct BoundingSphere
    {
        BoundingSphere()
            : center(glm::vec3(0.0f, 0.0f, 0.0f))
            , radius(0.0f)
        {}

        glm::vec3 center;
        f32 radius;
    };

    struct BoundingPlane
    {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct BoundingBox
    {
    };

    void update(RigidBody& _body, const float _deltaTime);
    void init_body(RigidBody& _body, const glm::vec3& _initialPosition = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat& _initialOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
    void update_body(RigidBody& _body, float _deltaTime, const std::vector<glm::vec3>& _forces, const glm::vec3& _torque);

    //void detect_collision(const BoundingSphere& _sphere, const BoundingPlane& _plane)
    //{

    //}
}