#include "collision.h"



namespace rain
{
    bool detect_collision(
        RigidBody& _bodyA, BoundingSphere& _boundA, Transform& _transformA,
        RigidBody& _bodyB, BoundingSphere& _boundB, Transform& _transformB)
    {
        float length = glm::length(_transformA.position - _transformB.position);
        float sumradius = _boundA.radius + _boundB.radius;

        if (length <= sumradius)
        {
            return true;
        }

        return false;
    }

    void collision_response(
        RigidBody& _bodyA, Transform& _transformA,
        RigidBody& _bodyB, Transform& _transformB)
    {
        glm::vec3 U1x, U1y, U2x, U2y, V1x, V1y, V2x, V2y;


        f32 m1, m2, x1, x2;
        glm::vec3 v1temp, v1, v2, v1x, v2x, v1y, v2y;

        glm::vec3 x = glm::normalize(_transformA.position - _transformB.position);

        v1 = _bodyA.velocity;
        x1 = glm::dot(x, v1);
        v1x = x * x1;
        v1y = v1 - v1x;
        m1 = _bodyA.mass;

        x = x * -1.0f;
        v2 = _bodyB.velocity;
        x2 = glm::dot(x, v2);
        v2x = x * x2;
        v2y = v2 - v2x;
        m2 = _bodyB.mass;

        _bodyA.momentum = glm::vec3(v1x*(m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y) * _bodyA.mass;
        _bodyB.momentum = glm::vec3(v1x*(2 * m1) / (m1 + m2) + v2x * (m2 - m1) / (m1 + m2) + v2y) * _bodyB.mass;
    }
}