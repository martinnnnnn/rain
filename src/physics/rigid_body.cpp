﻿#include "rigid_body.h"

#include <Windows.h>
#include <cmath>

namespace rain
{
    void update(RigidBodyPosition& _body, float _deltaTime)
    {
        std::vector<glm::vec3> forces;
        forces.push_back(glm::vec3(0.0f, -9.81f, 0.0f));

        update_body(_body, _deltaTime, forces);
    }

    void init_body(RigidBodyPosition& _body, const glm::vec3& _initialPosition)
    {
        _body.position = _initialPosition;
        _body.momentum = glm::vec3(5.0f, 15.0f, 0.0f);
        _body.mass = 3.0f;
        _body.mass_inverse = 1.0f / _body.mass;
    }

    void update_body(RigidBodyPosition& _body, float _deltaTime, const std::vector<glm::vec3>& _forces)
    {
        _body.force = glm::vec3(0.0f);
        for (int i = 0; i < _forces.size(); ++i)
        {
            _body.force += _forces[i];
        }

        _body.momentum += _body.force * _deltaTime;
        _body.velocity = _body.momentum * _body.mass_inverse;
        _body.position += _body.velocity * _deltaTime;
    }


    void init_orientation(RigidBodyOrientation& _body, const glm::quat& _initialRotation)
    {
        _body.orientation = _initialRotation;
        //_body.angularMomentum = glm::vec3(0.0f, 15.0f, 0.0f);
        _body.mass = 3.0f;
        _body.mass_inverse = 1.0f / _body.mass;
        _body.rotationInertia = powf((1.0f / 6.0f) * _body.size, 2.0f * _body.mass);
        _body.rotationInertiaInverse = 1.0f * _body.rotationInertia;
    }

    void update_orientation(RigidBodyOrientation& _body, float _deltaTime)
    {
        _body.torque = glm::vec3(20.0f, 0.0f, 0.0f) - _body.angularVelocity * 0.1f;
        
        _body.angularMomentum += _body.torque * _deltaTime;
        _body.angularVelocity = _body.angularMomentum * _body.rotationInertiaInverse;
        _body.spin = 0.5f * glm::quat(0, _body.angularVelocity.x, _body.angularVelocity.y, _body.angularVelocity.z) /** _body.orientation*/;
        _body.orientation += _body.spin * _deltaTime;

        //char buffer[512];
        //sprintf_s(buffer, "momentum : (%f,%f,%f)\nvelocity : (%f,%f,%f)\norentation : (%f,%f,%f,%f)\n\n",
        //    _body.angularMomentum.x, _body.angularMomentum.y, _body.angularMomentum.z,
        //    _body.angularVelocity.x, _body.angularVelocity.y, _body.angularVelocity.z,
        //    _body.spin.x, _body.spin.y, _body.spin.z, _body.spin.w);
        //OutputDebugStringA(buffer);
    }

}