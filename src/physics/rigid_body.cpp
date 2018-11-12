#include "rigid_body.h"

#include <Windows.h>
#include <cmath>

namespace rain
{

    void update(RigidBody& _body, const f32 _deltaTime)
    {
        // here we're adding some forces to move objects around 
        // this should really be done differenlty in the future -> springs, dampers, collision resolution, gravity, projectile, etc.

        // adding linear forces
        std::vector<glm::vec3> forces;
        forces.push_back(glm::vec3(0.0f, -9.81f, 0.0f));
        
        // adding torque
        glm::vec3 torque = glm::vec3(0.0f, 0.0f, -1.0f) - _body.angularVelocity * 0.2f;

        update_body(_body, _deltaTime, forces, torque);
    }

    void init_body(RigidBody& _body, const glm::vec3& _initialPosition, const glm::quat& _initialOrientation)
    {
        // position & co
        _body.position = _initialPosition;
        //_body.momentum = glm::vec3(5.0f, 15.0f, 0.0f);
        _body.momentum = glm::vec3(0.0f, 0.0f, 0.0f);
        static bool first = true;
        _body.mass = 100.0f;
        if (first)
        {
            first = false;
            _body.position = glm::vec3(0.0f, 12.0f, 0.0f);
            _body.mass = 1.0f;
        }
        _body.mass_inverse = 1.0f / _body.mass;
        
        // rotation & co
        _body.orientation = _initialOrientation;
        _body.size = 1.0f;
        _body.rotationInertia = (1.0f / 6.0f) * _body.mass * powf(_body.size, 2);
        _body.rotationInertiaInverse = 1.0f / _body.rotationInertia;
    }

    void update_body(RigidBody& _body, f32 _deltaTime, const std::vector<glm::vec3>& _forces, const glm::vec3& _torque)
    {
        // update position
        _body.force = glm::vec3(0.0f);
        for (u32 i = 0; i < _forces.size(); ++i)
        {
            _body.force += _forces[i];
        }

        _body.momentum += _body.force * _deltaTime;
        _body.velocity = _body.momentum * _body.mass_inverse;
        _body.position += _body.velocity * _deltaTime;

        // update orientation
        _body.torque = _torque;
        _body.angularMomentum += _body.torque * _deltaTime;
        _body.angularVelocity = _body.angularMomentum * _body.rotationInertiaInverse;
        _body.spin = 0.5f * glm::quat(0, _body.angularVelocity.x, _body.angularVelocity.y, _body.angularVelocity.z) * _body.orientation;
        _body.orientation += _body.spin * _deltaTime;
        _body.orientation = glm::normalize(_body.orientation);
    }
}