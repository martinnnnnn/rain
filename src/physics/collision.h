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
            : position(glm::vec3(0.0f, 0.0f, 0.0f))
            , radius(0.0f)
        {}

        glm::vec3 position;
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

    glm::vec3 project(const glm::vec3& u, const glm::vec3& v)
    {
        return v * (glm::dot(u, v) / glm::dot(v, v));
    }


    void detect_collision(BoundingSphere& _sphere1Bounding, BoundingSphere& _sphere2Bounding, RigidBody& _sphere1Body, RigidBody& _sphere2Body)
    {
        _sphere1Bounding.position = _sphere1Body.position;
        _sphere2Bounding.position = _sphere2Body.position;

        int rSquared = _sphere1Bounding.radius + _sphere2Bounding.radius;
        rSquared *= rSquared;

        glm::vec3 delta = _sphere1Bounding.position - _sphere2Bounding.position;
        float distance = glm::dot(delta, delta);

        if (distance < rSquared)
        {
            glm::vec3 nv1;
            glm::vec3 nv2;
            nv1 = _sphere1Body.momentum;
            nv1 += project(_sphere2Body.momentum, _sphere2Body.position - _sphere1Bounding.position);
            nv1 -= project(_sphere1Body.momentum, _sphere1Body.position - _sphere2Bounding.position);
            nv2 = _sphere2Body.momentum;
            nv2 += project(_sphere1Body.momentum, _sphere2Body.position - _sphere1Bounding.position);
            nv2 -= project(_sphere1Body.momentum, _sphere1Body.position - _sphere2Bounding.position);
            _sphere1Body.momentum = nv1;
            _sphere2Body.momentum = nv2;
        }

    }
}