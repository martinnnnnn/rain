#pragma once


#include "utility/incl_3d.h"
#include "utility/first_time_mcr.h"

#include <iostream>

namespace rain
{
    //class Transform
    //{
    //public:

    //    Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
    //        Position(position),
    //        Rotation(rotation),
    //        Scale(scale)
    //    {
    //    }

    //    Transform() :
    //        Position(),
    //        Rotation(glm::vec3(0)),
    //        Scale(1,1,1)
    //    {

    //    }

    //    void Translate(const glm::vec3& value)
    //    {
    //        Position += value;
    //    }

    //    void ChangeScale(const glm::vec3& value)
    //    {
    //        Scale += value;
    //    }

    //    void Rotate(const glm::vec3& axis, float _angle)
    //    {
    //        Rotation = Rotation * glm::angleAxis(glm::radians(_angle), axis);
    //    }

    //    void SetRotation(const glm::vec3& axis, float _angle)
    //    {
    //        Rotation = glm::angleAxis(glm::radians(_angle), axis);
    //    }

    //    glm::mat4 GetModelMatrix()
    //    {
    //        glm::mat4 translation = glm::translate(glm::mat4(1), Position);
    //        glm::mat4 rotation = glm::mat4_cast(Rotation);
    //        glm::mat4 scale = glm::scale(glm::mat4(1), Scale);

    //        return translation * rotation * scale;
    //    }

    //    glm::vec3 Position;
    //    glm::vec3 Scale;
    //    glm::quat Rotation;
    //};

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
