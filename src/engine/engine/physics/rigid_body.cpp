#include "rigid_body.h"


namespace rain::engine
{
    void apply_physics(RigidBody& _body, core::transform& _transform, const f32 _deltaTime)
    {
        if (!_body.infiniteMass)
        {
            // update position
            _body.momentum += _body.force * _deltaTime;
            _body.velocity = _body.momentum * _body.mass_inverse;
            _transform.position += _body.velocity * _deltaTime;

            // update orientation
            _body.angularMomentum += _body.torque * _deltaTime;
            _body.angularVelocity = _body.angularMomentum * _body.rotationInertiaInverse;
            _body.spin = 0.5f * glm::quat{ 0, _body.angularVelocity.x, _body.angularVelocity.y, _body.angularVelocity.z } * _transform.orientation;
            _transform.orientation += _body.spin * _deltaTime;
            _transform.orientation = glm::normalize(_transform.orientation);
        }

        _body.force = glm::vec3{};
        _body.torque = glm::vec3{};
    }
}