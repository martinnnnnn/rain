#pragma once


#include "gfx/gfx.h"
#include "ecs/ecs.h"

namespace rain
{
    struct DebugData
    {

    };



    void DrawDebugCube(bool _wireframe = false);
    void DrawDebugSphere(bool _wireframe = false);
    void DrawDebugPlane(bool _wireframe = false);
    void DrawDebugCylinder(bool _wireframe = false);
    
}