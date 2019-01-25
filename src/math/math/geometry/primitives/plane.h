#pragma once

//#include <glm/glm.hpp>

#include "vec3.h"

namespace rain::math
{
    struct Plane
    {
        Plane()
            : n(vec3{})
            , D(-dot(n, vec3{}))
        {}

        // init plane from point and normal
        Plane(const vec3& _p0, const vec3& _n)
            : n(_n)
            , D(-dot(n, _p0))
        {
        }

        // init plane from 3 points
        Plane(const vec3& _p0, const vec3& _p1, const vec3& _p2)
            : n(normalized(cross((_p1 - _p0), (_p2 - _p0))))
            , D(-dot(n, _p0))
        {
        }

        // plane normal
        vec3 n;
        // distance from the origin
        f32 D;
    };

    inline f32 distance_point_to_plane(const vec3& _point, const Plane& _plane)
    {
        return dot(_plane.n, _point) + _plane.D;
    }

    inline vec3 project_on_plane(const vec3& _point, const Plane& _plane)
    {
        return _point - (_plane.n * distance_point_to_plane(_point, _plane));
    }
}