#pragma once


#include "utility/incl_3d.h"

#include <iostream>

namespace rain
{
    class Transform
    {
    public:

        Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
            Position(position),
            Rotation(rotation),
            Scale(scale)
        {
        }

        Transform() :
            Position(),
            Rotation(),
            Scale()
        {

        }

        void Translate(const glm::vec3& value)
        {
            Position += value;
        }

        void ChangeScale(const glm::vec3& value)
        {
            Scale += value;
        }

        void Rotate(const glm::vec3& value)
        {
            Rotation += glm::quat(value);
        }

        glm::mat4 GetModelMatrix()
        {
            glm::mat4 translation = glm::translate(glm::mat4(1), Position);
            glm::mat4 rotation = glm::mat4_cast(Rotation);
            glm::mat4 scale = glm::scale(glm::mat4(1), Scale);

            return translation * rotation * scale;
        }

        glm::vec3 Position;
        glm::vec3 Scale;
        glm::quat Rotation;
    };
}