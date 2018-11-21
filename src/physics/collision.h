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
        BoundingPlane()
            : n(glm::vec3(0.0))
            , D(-glm::dot(n, glm::vec3(0.0)))
        {}

        BoundingPlane(const glm::vec3& _p0, const glm::vec3& _n)
            : n(_n)
            , D(-glm::dot(n,_p0))
        {
        }

        BoundingPlane(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2)
            : n(glm::cross((_p1 - _p0), glm::normalize(_p2 - _p0)))
            , D(-glm::dot(n, _p0))
        {
        }
        
        const f32 distance_to_point(const glm::vec3& _p) const
        {
            return glm::dot(n, _p) + D;
        }

        glm::vec3 project(glm::vec3 _point)
        {
            return _point - (distance_to_point(_point) * n);
        }

        glm::vec3 n;
        f32 D;
    };

    struct BoundingQuad
    {

    };

    struct BoundingBox
    {
    };

    struct HitInfo
    {
        bool hit;
        glm::vec3 positionObjA;
        glm::vec3 positionObjB;
        f32 normalizedTime;
        glm::vec3 contactPoint;
        glm::vec3 contactNormal;
    };


    bool get_quadratic_roots(const f32 _a, const f32 _b, const f32 _c, f32& _r1, f32& _r2);

    HitInfo detect_collision(BoundingSphere& _boundA, Transform& _transformA, BoundingSphere& _boundB, Transform& _transformB);
    HitInfo detect_collision( BoundingSphere& _sphereBound, Transform& _sphereTransform, BoundingPlane& _planeBound);

    void collision_response(RigidBody& _bodyA, Transform& _transformA, glm::vec3& _position);
    void collision_response(RigidBody& _bodyA, Transform& _transformA, RigidBody& _bodyB, Transform& _transformB);

}