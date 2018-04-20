#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Transform
{
public:
    
    Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
        _Position(position),
        _Rotation(rotation),
        _Scale(scale)
    {
    }

    Transform() :
        _Position(),
        _Rotation(),
        _Scale()
    {

    }

    void Translate(const glm::vec3& value)
    {
        _Position += value;
    }

    void Scale(const glm::vec3& value)
    {
        _Scale += value;
    }

    void Rotate(const glm::vec3& value)
    {
        _Rotation += glm::quat(value);
    }

    glm::mat4 GetModelMatrix()
    {
        glm::mat4 translation = glm::translate(glm::mat4(1), _Position);
        glm::mat4 rotation = glm::mat4_cast(_Rotation);
        glm::mat4 scale = glm::scale(glm::mat4(1), _Scale);

        return translation * rotation * scale;
    }

    glm::vec3 _Position;
    glm::vec3 _Scale;
    glm::quat _Rotation;
};