#include "rigid_body.h"

#include <Windows.h>
#include <cmath>

namespace rain
{

    void apply_physics(RigidBody& _body, Transform& _transform, const f32 _deltaTime)
    {
        // TODO (martin) : here we're adding some forces to move objects around 
        // this should really be done differenlty in the future -> springs, dampers, collision resolution, gravity, projectile, etc.

        // adding linear forces
        std::vector<glm::vec3> forces;
        forces.push_back(glm::vec3(0.0f, -9.81f, 0.0f));
        
        // adding torque
        glm::vec3 torque = glm::vec3(0.0f, 0.0f, -1.0f) - _body.angularVelocity * 0.2f;

        apply_physics(_body, _transform, _deltaTime, forces, torque);
    }

    void apply_physics(RigidBody& _body, Transform& _transform, f32 _deltaTime, const std::vector<glm::vec3>& _forces, const glm::vec3& _torque)
    {
        // update position
        _body.force = glm::vec3(0.0f);
        for (u32 i = 0; i < _forces.size(); ++i)
        {
            _body.force += _forces[i];
        }

        _body.momentum += _body.force * _deltaTime;
        _body.velocity = _body.momentum * _body.mass_inverse;
        _transform.currentPosition += _body.velocity * _deltaTime;

        // update orientation
        _body.torque = _torque;
        _body.angularMomentum += _body.torque * _deltaTime;
        _body.angularVelocity = _body.angularMomentum * _body.rotationInertiaInverse;
        _body.spin = 0.5f * glm::quat(0, _body.angularVelocity.x, _body.angularVelocity.y, _body.angularVelocity.z) * _transform.currentOrientation;
        _transform.currentOrientation += _body.spin * _deltaTime;
        _transform.currentOrientation = glm::normalize(_transform.currentOrientation);
    }
}