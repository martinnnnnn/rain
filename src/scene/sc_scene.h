#pragma once


#include "ecs/ecs.h"


namespace rain
{
    struct Scene
    {
        char* name;
        char* path;
        //EntityContainer* entityContainer;
    };

    void SerializeScene(Scene* _scene, const char* _path = "");
}