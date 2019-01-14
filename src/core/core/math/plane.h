#pragma once

#include <glm/glm.hpp>

#include "core/types.h"

namespace rain::core::math
{
    struct Plane
    {
        Plane()
            : n(glm::vec3(0.0))
            , D(-glm::dot(n, glm::vec3(0.0)))
        {}

        // init plane from point and normal
        Plane(const glm::vec3& _p0, const glm::vec3& _n)
            : n(_n)
            , D(-glm::dot(n, _p0))
        {
        }

        // init plane from 3 points
        Plane(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2)
            : n(glm::normalize(glm::cross((_p1 - _p0), (_p2 - _p0))))
            , D(-glm::dot(n, _p0))
        {
        }

        // plane normal
        glm::vec3 n;
        // distance from the origin
        f32 D;
    };

    inline f32 distance_point_to_plane(const glm::vec3& _point, const Plane& _plane)
    {
        return glm::dot(_plane.n, _point) + _plane.D;
    }

    inline glm::vec3 project_on_plane(const glm::vec3& _point, const Plane& _plane)
    {
        return _point - (distance_point_to_plane(_point, _plane) * _plane.n);
    }
}