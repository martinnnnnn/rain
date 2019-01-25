#pragma once


#include "math/math.h"
#include "core/core.h"


namespace rain::engine
{
    using vec3 = math::vec3;
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
        vec3 anchorPointA;
        vec3 anchorPointB;
        f32 distance;
        f32 k;
        f32 b;
    };
}