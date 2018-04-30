#pragma once

#include "utility/stdincl_3d.h"

namespace rain
{
    class Light
    {
    public:

        enum class Type
        {
            DIRECTIONAL,
            POINT,
            SPOT
        };

        Type Type;

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


