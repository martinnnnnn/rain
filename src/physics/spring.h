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
            , anchorPoint(0.0)
        {}
        u32 entity;
        f32 k;
        f32 b;
        glm::vec3 anchorPoint;
    };

    struct Spring2
    {
        Spring2()
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