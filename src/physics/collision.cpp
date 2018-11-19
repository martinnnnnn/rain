#include "collision.h"



namespace rain
{
    HitInfo detect_collision(
        BoundingSphere& _boundA, Transform& _transformA,
        BoundingSphere& _boundB, Transform& _transformB)
    {
        HitInfo info{};
        float length = glm::length(_transformA.position - _transformB.position);
        float sumradius = _boundA.radius + _boundB.radius;

        if (length <= sumradius)
        {
            info.hit = true;
        }

        return info;
    }

    HitInfo detect_collision(BoundingSphere& _sphereBound, Transform& _sphereTransform, BoundingPlane& _planeBound)
    {
        HitInfo info{};

        const f32 d0 = _planeBound.distance_to_point(_sphereTransform.previousPosition);
        const f32 d1 = _planeBound.distance_to_point(_sphereTransform.position);

        // check if it was touching on previous frame
        if (fabs(d0) <= _sphereBound.radius)
        {
            info.hit = true;
            info.positionObjA = _sphereTransform.previousPosition;
            info.normalizedTime = 0;
        }
        // check if the sphere penetrated during this frame
        else if (d0 > _sphereBound.radius && d1 < _sphereBound.radius)
        {
            info.hit = true;
            info.normalizedTime = (d0 - _sphereBound.radius) / (d0 - d1);	//normalized time
            info.positionObjA = (1 - info.normalizedTime)* _sphereTransform.previousPosition + info.normalizedTime * _sphereTransform.position;	//point of first contact
        }

        return info;
    }

    void collision_response(RigidBody& _bodyA, Transform& _transformA, glm::vec3& _position)
    {
        glm::vec3 x = glm::normalize(_transformA.position - _position);

        glm::vec3 v1 = _bodyA.velocity;
        f32 x1 = glm::dot(x, v1);
        glm::vec3 v1x = x * x1;
        glm::vec3 v1y = v1 - v1x;
        f32 m1 = _bodyA.mass;

        x = x * -1.0f;
        glm::vec3 v2 = glm::vec3(0.0f,0.0f,0.0f);
        f32 x2 = glm::dot(x, v2);
        glm::vec3 v2x = x * x2;
        f32 m2 = 100.0f;

        _bodyA.momentum = glm::vec3(v1x*(m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y) * _bodyA.mass;
    }

    void collision_response(
        RigidBody& _bodyA, Transform& _transformA,
        RigidBody& _bodyB, Transform& _transformB)
    {
        glm::vec3 x = glm::normalize(_transformA.position - _transformB.position);

        glm::vec3 v1 = _bodyA.velocity;
        f32 x1 = glm::dot(x, v1);
        glm::vec3 v1x = x * x1;
        glm::vec3 v1y = v1 - v1x;
        f32 m1 = _bodyA.mass;

        x = x * -1.0f;
        glm::vec3 v2 = _bodyB.velocity;
        f32 x2 = glm::dot(x, v2);
        glm::vec3 v2x = x * x2;
        glm::vec3 v2y = v2 - v2x;
        f32 m2 = _bodyB.mass;

        _bodyA.momentum = glm::vec3(v1x*(m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y) * _bodyA.mass;
        _bodyB.momentum = glm::vec3(v1x*(2 * m1) / (m1 + m2) + v2x * (m2 - m1) / (m1 + m2) + v2y) * _bodyB.mass;
    }
}