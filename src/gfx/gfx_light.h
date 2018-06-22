#pragma once

#include <stdlib.h>

#include "utility/incl_3d.h"
#include "ecs/ecs_component.h"
#include "core/types.h"


namespace rain
{
    enum class LightType
    {
        UNKNOWN = 0,
        DIRECTIONAL,
        POINT,
        SPOT
    };

    struct Light : Component
    {
        LightType lightType;

        glm::vec3 ambient;
        glm::vec3 direction;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float constant;
        float linear;
        float quadratic;
        float cutOff;
        float outerCutOff;
    };

    Light* CreateLight();

    //struct LightPoolElement
    //{
    //    union Elem
    //    {
    //        Light* light;
    //        LightPoolElement* next;
    //    };

    //    Elem element;
    //};

    //struct LightPool
    //{
    //    LightPoolElement** lights;
    //    LightPoolElement* first;
    //    u32 capacity;
    //    u32 resizeStep;
    //};

    //LightPool* CreateLightPool(u32 _capacity = 100, u32 _resizeStep = 10);

    //Light* GetLight(LightPool* _pool);
    //void FreeLight(LightPool* _pool, LightPoolElement* _element);
}


