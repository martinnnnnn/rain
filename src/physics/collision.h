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
        f32 D;
        glm::vec3 n;
        BoundingPlane(const glm::vec3& _p0, const glm::vec3& _n)
            : n(n)
            , D(-glm::dot(n,_p0))
        {}

        BoundingPlane(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2)
            : n(glm::cross((_p1 - _p0), glm::normalize(_p2 - _p0)))
            , D(-glm::dot(n, _p0))
        {}
        
        const f32 distance_to_point(const glm::vec3& _p) const
        {
            return glm::dot(n, _p) + D;
        }
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