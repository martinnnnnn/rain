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

        Plane(const glm::vec3& _p0, const glm::vec3& _n)
            : n(_n)
            , D(-glm::dot(n, _p0))
        {
        }

        Plane(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2)
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
}