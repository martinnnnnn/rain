#pragma once

#include <iostream>

#include "utility/incl_3d.h"
#include "utility/first_time_mcr.h"
#include "ecs/ecs_component.h"


namespace rain
{
    struct Transform : Component
    {
        glm::vec3 position;
        glm::vec3 scale;
        glm::quat rotation;
    };

    Transform* CreateTransform();

    void Translate(Transform* _transform, const glm::vec3& _amount);
    void SetLocation(Transform* _transform, const glm::vec3& _newLocation);
    void Scale(Transform* _transform, const glm::vec3& _amount);
    void SetScale(Transform* _transform, const glm::vec3& _newScale);
    void Rotate(Transform* _transform, const glm::vec3& _axis, float _angle);
    void SetRotation(Transform* _transform, const glm::vec3& _axis, float _angle);

    glm::mat4 GetModelMatrix(const Transform* _transform);
}
