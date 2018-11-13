#include "rigid_body.h"

#include <Windows.h>
#include <cmath>

namespace rain
{

    /*void apply_spring_rope(const Spring& _spring, RigidBody& _bodyA, RigidBody& _bodyB, Transform& _transform)
    {
        _body.force += -_spring.k * _transform.position - _spring.b * _body.velocity;
    }*/


    void apply_physics(RigidBody& _body, Transform& _transform, const f32 _deltaTime)
    {
        /*if (_body.applyGravity)
        {
            apply_gravity(_body);
        }
        apply_spring(spring, _body, _transform);*/





        // TODO (martin) : here we're adding some forces to move objects around 
        // this should really be done differenlty in the future -> springs, dampers, collision resolution, gravity, projectile, etc.
        // adding linear forces
        std::vector<glm::vec3> forces;
        //forces.push_back(glm::vec3(0.0f, -9.81f, 0.0f));

        // adding torque
        glm::vec3 torque = glm::vec3(0.0f, 0.0f, 0.0f);
        //glm::vec3 torque = glm::vec3(0.0f, 0.0f, -1.0f) - _body.angularVelocity * 0.2f;

        apply_physics(_body, _transform, _deltaTime, forces, torque);
    }

    void apply_physics(RigidBody& _body, Transform& _transform, f32 _deltaTime, const std::vector<glm::vec3>& _forces, const glm::vec3& _torque)
    {
        // TODO (martin) : apply gravity should actually just ignore gravity, not all forces
        if (_body.infiniteMass)
        {
            return;
        }

        // update position
        //
        //for (u32 i = 0; i < _forces.size(); ++i)
        //{
        //    _body.force += _forces[i];
        //}

        _body.momentum += _body.force * _deltaTime;
        _body.velocity = _body.momentum * _body.mass_inverse;
        _transform.position += _body.velocity * _deltaTime;

        // update orientation
        _body.torque = _torque;
        _body.angularMomentum += _body.torque * _deltaTime;
        _body.angularVelocity = _body.angularMomentum * _body.rotationInertiaInverse;
        _body.spin = 0.5f * glm::quat(0, _body.angularVelocity.x, _body.angularVelocity.y, _body.angularVelocity.z) * _transform.orientation;
        _transform.orientation += _body.spin * _deltaTime;
        _transform.orientation = glm::normalize(_transform.orientation);

        _body.force = glm::vec3(0.0f);
    }
}