#pragma once


#include "gfx/gfx.h"
#include "ecs/ecs.h"

namespace rain
{
    struct Renderer : public System
    {
        Camera* camera;
        Vector<Light*> lights;
    };


    void Draw(System* _render, Renderer* _renderer)
    {

        for (int i = 0; i < _render->entities.size; ++i)
        {
            
        }
    }
}