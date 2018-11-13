#include "physics.h"

#include <Windows.h>

#include "math/transform.h"

namespace rain
{

    void Physics::apply_gravity(RigidBody& _body)
    {
        if (_body.applyGravity)
        {
            _body.force += glm::vec3(0.0f, -9.81f, 0.0f);
        }
    }

    void Physics::apply_spring(const Spring& _spring, const Transform& _transform, RigidBody& _body)
    {
        _body.force += -_spring.k * _transform.position - _spring.b * _body.velocity;
    }

    void Physics::apply_spring(const Spring& _spring, const Transform& _transform, RigidBody& _body)
    {

    }

    void Physics::update(RigidBody& _body, Transform& _transform, float _deltaTime)
    {
        _transform.previousPosition = _transform.position;
        _transform.previousOrientation = _transform.orientation;

        apply_physics(_body, _transform, _deltaTime);
    }
}