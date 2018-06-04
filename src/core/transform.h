#pragma once


#include "utility/incl_3d.h"
#include "utility/first_time_mcr.h"

#include <iostream>

namespace rain
{
    struct TransformS
    {
        TransformS();

        glm::vec3 position;
        glm::vec3 scale;
        glm::quat rotation;
    };

    void Translate(TransformS& _transform, const glm::vec3& _amount);
    void SetLocation(TransformS& _transform, const glm::vec3& _newLocation);
    void Scale(TransformS& _transform, const glm::vec3& _amount);
    void SetScale(TransformS& _transform, const glm::vec3& _newScale);
    void Rotate(TransformS& _transform, const glm::vec3& _axis, float _angle);
    void SetRotation(TransformS& _transform, const glm::vec3& _axis, float _angle);

    glm::mat4 GetModelMatrix(const TransformS& _transform);
}
