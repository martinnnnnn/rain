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

    bool detect_collision(BoundingSphere& _bound1, BoundingSphere& _bound2, RigidBody& _body1, RigidBody& _body2);
    void collision_response(BoundingSphere& _bound1, BoundingSphere& _bound2, RigidBody& _body1, RigidBody& _body2);
}