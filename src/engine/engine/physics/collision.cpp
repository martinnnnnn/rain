#include "collision.h"

#include "engine/core/context.h"
#include "engine/core/logger.h"

/*
1. generate convex hulls                                            X
2. get collision detection working with EJK                         X
3. concave hull -> convex hull algorithm implementation             X
4. collision response with manifold etc.                            X
5. collision detection and response simplified for basic shapes     X
*/



namespace rain::engine
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

    HitInfo detect_collision_sphere_plane(Sphere& _sphere, Transform& _transform, Plane& _plane)
    {
        HitInfo info{};

        const f32 d0 = core::math::distance_point_to_plane(_transform.lastPosition, _plane);
        const f32 d1 = core::math::distance_point_to_plane(_transform.position, _plane);

        // check if it was touching on previous frame
        if (fabs(d0) <= _sphere.radius)
        {
            info.hit = true;
            //info.positionObjA = _transform.lastPosition;
            info.normalizedTime = 0;
        }
        // check if the sphere penetrated during this frame
        else if (d0 > _sphere.radius && d1 < _sphere.radius)
        {
            info.hit = true;
            info.normalizedTime = (d0 - _sphere.radius) / (d0 - d1);	//normalized time
            //info.positionObjA = (1 - info.normalizedTime) * _transform.lastPosition + info.normalizedTime * _transform.position;	//point of first contact
        }
        return info;
    }

    glm::vec2 detect_collision_ray_sphere(const Ray& _ray, const Sphere& _sphere)
    {
        glm::vec3 oc = _ray.origin - _sphere.offset;
        float b = glm::dot(oc, _ray.direction);
        float c = glm::dot(oc, oc) - _sphere.radius * _sphere.radius;
        float h = b * b - c;
        if (h < 0.0) return glm::vec2(-1.0f); // no intersection
        h = sqrt(h);
        return glm::vec2(-b - h, -b + h);
    }


    
    HitInfo detect_collision_sphere(const Sphere& _sphereA, const Transform& _transformA, const Sphere& _sphereB, const Transform& _transformB)
    {
        HitInfo info{};

        const glm::vec3 va = _transformA.position - _transformA.lastPosition;
        const glm::vec3 vb = _transformB.position - _transformB.lastPosition;
        const glm::vec3 AB = _transformB.lastPosition - _transformA.lastPosition;
        const glm::vec3 vab = va - vb;

        const f32 rab = _sphereA.radius + _sphereB.radius;
        const f32 a = glm::dot(vab, vab);
        const f32 b = -2.0f * glm::dot(AB, vab);
        const f32 c = glm::dot(AB, AB) - rab * rab;

        f32 u1;
        if (glm::dot(AB, AB) <= rab * rab)
        {
            info.hit = true;
            info.normalizedTime = 0;
        }
        if (core::math::get_quadratic_roots(a, b, c, info.normalizedTime, u1))
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

    HitInfo detect_collision_gjk(const MeshBound& _verticesA, const Transform& _transformA, const MeshBound& _verticesB, const Transform& _transformB)
    {

        return HitInfo {};
    }

}