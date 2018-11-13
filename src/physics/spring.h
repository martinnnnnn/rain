#pragma once


#include "core/types.h"


namespace rain
{
    struct Spring
    {
        Spring()
            : entity(0)
            , k(15.0f)
            , b(0.1f)
        {}
        u32 entity;
        f32 k;
        f32 b;
    };

    struct SpringRope
    {
        SpringRope()
            : entityA(0)
            , entityB(0)
            , distance(0.0f)
            , k(15.0f)
            , b(0.1f)
        {}

        u32 entityA;
        u32 entityB;
        float distance;
        f32 k;
        f32 b;
    };

}