#include "physics.h"

#include <Windows.h>

#include "math/transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

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
        glm::vec3 point_velocity = _body.velocity + glm::cross(_body.angularVelocity, (_spring.anchorPoint, _transform.position));
        glm::vec3 spring_force = -_spring.k * _transform.position - _spring.b * point_velocity;

        _body.force += spring_force;
        _body.torque += glm::cross(spring_force, (_spring.anchorPoint, _transform.position));
    }

    void Physics::apply_spring(const SpringRope& _spring, const Transform& _transformA, RigidBody& _bodyA, const Transform& _transformB, RigidBody& _bodyB)
    {
        f32 distance = glm::distance(_transformA.position, _transformB.position);
        glm::vec3 directionAtoB = glm::normalize(_transformB.position - _transformA.position);

        _bodyA.force += -_spring.k * (distance - _spring.distance) * -directionAtoB - _spring.b * _bodyA.velocity;
        _bodyB.force += -_spring.k * (distance - _spring.distance) * directionAtoB - _spring.b * _bodyB.velocity;
    }


    void Physics::apply_spring(const Spring2& _spring, const Transform& _transformA, RigidBody& _bodyA, const Transform& _transformB, RigidBody& _bodyB)
    {
        glm::vec3 position_anchorA = glm::translate(glm::mat4(1), _transformA.position) * glm::mat4_cast(_transformA.orientation) * glm::vec4(_spring.anchorPointA, 1.0);
        glm::vec3 position_anchorB = glm::translate(glm::mat4(1), _transformB.position) * glm::mat4_cast(_transformB.orientation) * glm::vec4(_spring.anchorPointB, 1.0);

        glm::vec3 point_velocityA = _bodyA.velocity + glm::cross(_bodyA.angularVelocity, (position_anchorA, _transformA.position));
        glm::vec3 point_velocityB = _bodyB.velocity + glm::cross(_bodyB.angularVelocity, (position_anchorB, _transformB.position));

        f32 distance = glm::distance(position_anchorA, position_anchorB);
        glm::vec3 directionAtoB = glm::normalize(position_anchorB - position_anchorA);
        glm::vec3 spring_forceA = -_spring.k * (distance - _spring.distance) * -directionAtoB - _spring.b * point_velocityA;
        glm::vec3 spring_forceB = -_spring.k * (distance - _spring.distance) * directionAtoB - _spring.b * point_velocityB;

        _bodyA.force += spring_forceA;
        _bodyB.force += spring_forceB;
        _bodyA.torque += glm::cross(spring_forceA, (position_anchorA, _transformA.position));
        _bodyB.torque += glm::cross(spring_forceB, (position_anchorB, _transformB.position));

        //char buffer[500];
        //sprintf_s(buffer, 500, "torqueA : (%f,%f,%f), torqueB : (%f,%f,%f)\n", _bodyA.torque.x, _bodyA.torque.y, _bodyA.torque.z, _bodyB.torque.x, _bodyB.torque.y, _bodyB.torque.z);
        //OutputDebugString(buffer);
    }


    void Physics::update(RigidBody& _body, Transform& _transform, float _deltaTime)
    {
        _transform.previousPosition = _transform.position;
        _transform.previousOrientation = _transform.orientation;

        apply_physics(_body, _transform, _deltaTime);
    }
}