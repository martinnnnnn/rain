#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "core/types.h"
#include "physics/rigid_body.h"

namespace rain
{
    struct BoundingSphere
    {
        BoundingSphere()
            : offset(glm::vec3(0.0f, 0.0f, 0.0f))
            , radius(0.0f)
        {}

        // offset from the center of the object - which must have a transform
        glm::vec3 offset;
        f32 radius;
    };

    struct BoundingPlane
    {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct BoundingQuad
    {

    };

    struct BoundingBox
    {
    };

    struct collision
    {

    };

    bool detect_collision(
        RigidBody& _bodyA, BoundingSphere& _boundA, Transform& _transformA,
        RigidBody& _bodyB, BoundingSphere& _boundB, Transform& _transformB);

    void collision_response(
        RigidBody& _bodyA, Transform& _transformA,
        RigidBody& _bodyB, Transform& _transformB);
}