#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "core/types.h"

namespace rain
{

    struct RigidBodyPosition
    {
        RigidBodyPosition()
            : position(glm::vec3(0.0f, 0.0f, 0.0f))
            , momentum(glm::vec3(0.0f, 0.0f, 0.0f))
            , velocity(glm::vec3(0.0f, 0.0f, 0.0f))
            , force(glm::vec3(0.0f, 0.0f, 0.0f))
            , mass(1.0f)
            , mass_inverse(1.0f / mass)
        {}

        glm::vec3 position;
        glm::vec3 momentum;
        glm::vec3 velocity;
        glm::vec3 force;

        float mass;
        float mass_inverse;
    };

    struct RigidBodyOrientation
    {
        RigidBodyOrientation()
            : orientation(glm::quat())
            , angularMomentum(glm::vec3(0.0f, 0.0f, 0.0f))
            , angularVelocity(glm::vec3(0.0f, 0.0f, 0.0f))
            , torque(glm::vec3(0.0f, 0.0f, 0.0f))
            , spin(glm::quat())
            , mass(1.0f)
            , size(1.0f)
            , mass_inverse(1.0f / mass)
            , rotationInertia(powf(6.0f * size, 2.0f * mass))
            , rotationInertiaInverse(1.0f / rotationInertia)
        {}

        glm::quat orientation;
        glm::vec3 angularMomentum;
        glm::vec3 torque;

        glm::quat spin;
        glm::vec3 angularVelocity;

        float mass;
        float size;
        float mass_inverse;
        float rotationInertia; // ⁄6 x size^2 x mass
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

    void update(RigidBodyPosition& _body, float _deltaTime);
    void init_body(RigidBodyPosition& _body, const glm::vec3& _initialPosition = glm::vec3(0.0f, 0.0f, 0.0f));
    void update_body(RigidBodyPosition& _body, float _deltaTime, const std::vector<glm::vec3>& _forces);
    void init_orientation(RigidBodyOrientation& _body, const glm::quat& _initialRotation);
    void update_orientation(RigidBodyOrientation& _body, float _deltaTime);



    //void detect_collision(const BoundingSphere& _sphere, const BoundingPlane& _plane)
    //{

    //}
}