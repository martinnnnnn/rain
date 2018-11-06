#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "core/types.h"


namespace rain
{
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

    struct BoundingQuad
    {

    };

    struct BoundingBox
    {
    };

    struct collision
    {

    };


    //void detect_collision(const BoundingSphere& _sphere, const BoundingPlane& _plane)
    //{

    //}
}