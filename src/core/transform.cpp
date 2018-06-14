#include "transform.h"




namespace rain
{
    /*Transform::Transform() :
        position(0,0,0),
        scale(1,1,1),
        rotation(glm::vec3(0,0,0))
    {

    }
*/

    Transform* GetNewHeapTransform()
    {
        Transform* t = (Transform*)malloc(sizeof(Transform));
        t->type = Component::Type::TRANSFORM;
        t->position = glm::vec3(0, 0, 0);
        t->scale = glm::vec3(1, 1, 1);
        t->rotation = glm::quat(glm::vec3(0, 0, 0));
        return t;
    }

    Transform GetNewStackTransform()
    {
        Transform t = {};
        t.type = Component::Type::TRANSFORM;
        t.position = glm::vec3(0, 0, 0);
        t.scale = glm::vec3(1, 1, 1);
        t.rotation = glm::quat(glm::vec3(0, 0, 0));
        return t;
    }

    Movement* GetNewMovement()
    {
        Movement* mov = (Movement*)malloc(sizeof(Movement));
        mov->type = Component::Type::MOVEMENT;
        mov->direction = glm::vec3(0, 0, 0);
        mov->speed = 0.0f;
        return mov;
    }

    void Translate(Transform& _transform, const glm::vec3& _amount)
    {
        _transform.position += _amount;
    }

    void SetLocation(Transform& _transform, const glm::vec3& _newLocation)
    {
        _transform.position = _newLocation;
    }

    void Scale(Transform& _transform, const glm::vec3& _amount)
    {
        _transform.scale += _amount;
    }

    void SetScale(Transform& _transform, const glm::vec3& _newScale)
    {
        _transform.scale = _newScale;
    }

    void Rotate(Transform& _transform, const glm::vec3& _axis, float _angle)
    {
        _transform.rotation = _transform.rotation * glm::angleAxis(glm::radians(_angle), _axis);
    }

    void SetRotation(Transform& _transform, const glm::vec3& _axis, float _angle)
    {
        _transform.rotation = glm::angleAxis(glm::radians(_angle), _axis);
    }

    glm::mat4 GetModelMatrix(const Transform& _transform)
    {
        glm::mat4 translation = glm::translate(glm::mat4(1), _transform.position);
        glm::mat4 rotation = glm::mat4_cast(_transform.rotation);
        glm::mat4 scale = glm::scale(glm::mat4(1), _transform.scale);

        return translation * rotation * scale;
    }

}