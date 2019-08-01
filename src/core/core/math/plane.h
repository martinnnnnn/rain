#pragma once

#include "glm.hpp"

namespace rain::core
{
    struct plane
    {
        plane()
            : n(glm::vec3{})
            , D(-glm::dot(n, glm::vec3{}))
        {}

        // init plane from point and normal
        plane(const glm::vec3& _p0, const glm::vec3& _n)
            : n(_n)
            , D(-glm::dot(n, _p0))
        {
        }

        // init plane from 3 points
        plane(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2)
            : n(glm::normalize(cross((_p1 - _p0), (_p2 - _p0))))
            , D(-glm::dot(n, _p0))
        {
        }

        // plane normal
        glm::vec3 n;
        // distance from the origin
        f32 D;
    };

    inline f32 distance_point_to_plane(const glm::vec3& _point, const plane& _plane)
    {
        return glm::dot(_plane.n, _point) + _plane.D;
    }

    inline glm::vec3 project_on_plane(const glm::vec3& _point, const plane& _plane)
    {
        return _point - (_plane.n * distance_point_to_plane(_point, _plane));
    }
}