#pragma once

#include "glm.hpp"
#include "core/core.h"


namespace rain::engine
{
    struct Spring
    {
        Spring()
            : entityA(0)
            , entityB(0)
            , anchorPointA{}
            , anchorPointB{}
            , distance(0.0f)
            , k(15.0f)
            , b(0.1f)
        {}

        u32 entityA;
        u32 entityB;
        glm::vec3 anchorPointA;
        glm::vec3 anchorPointB;
        f32 distance;
        f32 k;
        f32 b;
    };
}