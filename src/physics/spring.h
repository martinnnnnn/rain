#pragma once


#include "core/types.h"


namespace rain
{
    struct Spring
    {
        Spring()
            : entityA(0)
            , entityB(0)
            , anchorPointA(0.0)
            , anchorPointB(0.0)
            , distance(0.0f)
            , k(15.0f)
            , b(0.1f)
        {}

        u32 entityA;
        u32 entityB;
        glm::vec3 anchorPointA;
        glm::vec3 anchorPointB;
        float distance;
        f32 k;
        f32 b;
    };
}