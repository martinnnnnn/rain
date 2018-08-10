#pragma once


#include "gfx/gfx.h"
#include "ecs/ecs.h"

namespace rain
{
    struct Renderer
    {
        Camera* camera;
        Vector<Light*> lights;
    };



    void DrawDebugCube(bool _wireframe);
    void DrawDebugSphere(bool _wireframe);
    void DrawDebugPlane(bool _wireframe);
    void DrawDebugCylinder(bool _wireframe);
    


}