#include "collision.h"

#include <cmath>

namespace rain
{


    

    //HitInfo detect_collision(BoundingSphere& _boundA, Transform& _transformA, BoundingSphere& _boundB, Transform& _transformB)
    //{
    //    HitInfo info{};
    //    float length = glm::length(_transformA.position - _transformB.position);
    //    float sumradius = _boundA.radius + _boundB.radius;

    //    if (length <= sumradius)
    //    {
    //        info.hit = true;
    //    }

    //    return info;
    //}

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
            info.positionObjA = (1 - info.normalizedTime) * _sphereTransform.previousPosition + info.normalizedTime * _sphereTransform.position;	//point of first contact
        }

        return info;
    }

    bool get_quadratic_roots(const f32 _a, const f32 _b, const f32 _c, f32& _r1, f32& _r2)
    {
        const f32 q = (_b * _b) - (4 * _a * _c);
        if (q >= 0)
        {
            const f32 sq = sqrt(q);
            const f32 d = 1.0f / (2.0f * _a);
            _r1 = (-_b + sq) * d;
            _r2 = (-_b - sq) * d;
            return true;
        }
        return false;
    }

    HitInfo detect_collision(BoundingSphere& _boundA, Transform& _transformA, BoundingSphere& _boundB, Transform& _transformB)
    {
        HitInfo info {};

        //printf("A: (%f, %f, %f), B: (%f, %f, %f)\n",
        //    _transformA.position.x, _transformA.position.y, _transformA.position.z,
        //    _transformB.position.x, _transformB.position.y, _transformB.position.z);

        const glm::vec3 va = _transformA.position - _transformA.previousPosition;
        const glm::vec3 vb = _transformB.position - _transformB.previousPosition;
        const glm::vec3 AB = _transformB.previousPosition - _transformA.previousPosition;
        const glm::vec3 vab = vb - va;

        const f32 rab = _boundA.radius + _boundB.radius;
        const f32 a = glm::dot(vab, vab);
        const f32 b = 2 * glm::dot(vab, AB);
        const f32 c = glm::dot(AB, AB) - rab * rab;

        f32 u1;
        if (c <= 0)
        {
            info.hit = true;
            info.normalizedTime = 0;
        }
        else if (get_quadratic_roots(a, b, c, info.normalizedTime, u1))
        {
            info.hit = true;
            if (info.normalizedTime > u1)
            {
                info.normalizedTime = u1;
            }
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
        glm::vec3 v2 = glm::vec3(0.0f, 0.0f, 0.0f);
        f32 x2 = glm::dot(x, v2);
        glm::vec3 v2x = x * x2;
        f32 m2 = 100.0f;

        _bodyA.momentum = glm::vec3(v1x*(m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y) * _bodyA.mass;
    }

    void collision_response(RigidBody& _bodyA, Transform& _transformA, RigidBody& _bodyB, Transform& _transformB)
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