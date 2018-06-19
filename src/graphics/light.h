#pragma once

#include "utility/incl_3d.h"
#include "ecs/ecs_component.h"

namespace rain
{
    enum class LightType
    {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    struct Light : Component
    {
        LightType lighttype;

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
}


