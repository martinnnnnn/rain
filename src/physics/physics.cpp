#include "physics.h"

#include <Windows.h>

#include "math/transform.h"

namespace rain
{

    void Physics::update(RigidBody& _body, Transform& _transform, float _deltaTime)
    {
        _transform.previousPosition = _transform.currentPosition;
        _transform.previousOrientation = _transform.currentOrientation;
        apply_physics(_body, _transform, _deltaTime);
    }
}