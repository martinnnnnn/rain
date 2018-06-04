#include "transform.h"




namespace rain
{
    TransformS::TransformS() :
        position(0,0,0),
        scale(1,1,1),
        rotation(glm::vec3(0,0,0))
    {

    }

    void Translate(TransformS& _transform, const glm::vec3& _amount)
    {
        _transform.position += _amount;
    }

    void SetLocation(TransformS& _transform, const glm::vec3& _newLocation)
    {
        _transform.position = _newLocation;
    }

    void Scale(TransformS& _transform, const glm::vec3& _amount)
    {
        _transform.scale += _amount;
    }

    void SetScale(TransformS& _transform, const glm::vec3& _newScale)
    {
        _transform.scale = _newScale;
    }

    void Rotate(TransformS& _transform, const glm::vec3& _axis, float _angle)
    {
        _transform.rotation = _transform.rotation * glm::angleAxis(glm::radians(_angle), _axis);
    }

    void SetRotation(TransformS& _transform, const glm::vec3& _axis, float _angle)
    {
        _transform.rotation = glm::angleAxis(glm::radians(_angle), _axis);
    }

    glm::mat4 GetModelMatrix(const TransformS& _transform)
    {
        glm::mat4 translation = glm::translate(glm::mat4(1), _transform.position);
        glm::mat4 rotation = glm::mat4_cast(_transform.rotation);
        glm::mat4 scale = glm::scale(glm::mat4(1), _transform.scale);

        return translation * rotation * scale;
    }

}