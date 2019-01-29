#include "collision.h"

#include "engine/core/context.h"

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
    //    float length = length(_transformA.position - _transformB.position);
    //    float sumradius = _boundA.radius + _boundB.radius;

    //    if (length <= sumradius)
    //    {
    //        info.hit = true;
    //    }

    //    return info;
    //}
    using namespace rain::math;


    HitInfo detect_collision_sphere_plane(Sphere& _sphere, Transform& _transform, Plane& _plane)
    {
        HitInfo info{};

        const f32 d0 = math::distance_point_to_plane(_transform.lastPosition, _plane);
        const f32 d1 = math::distance_point_to_plane(_transform.position, _plane);

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

    vec2 detect_collision_ray_sphere(const Ray& _ray, const Sphere& _sphere)
    {
        vec3 oc = _ray.origin - _sphere.offset;
        float b = dot(oc, _ray.direction);
        float c = dot(oc, oc) - _sphere.radius * _sphere.radius;
        float h = b * b - c;
        if (h < 0.0) return vec2{ -1.0f, -1.0f }; // no intersection
        h = sqrt(h);
        return vec2{ -b - h, -b + h };
    }


    
    HitInfo detect_collision_sphere(const Sphere& _sphereA, const Transform& _transformA, const Sphere& _sphereB, const Transform& _transformB)
    {
        HitInfo info{};

        const vec3 va = _transformA.position - _transformA.lastPosition;
        const vec3 vb = _transformB.position - _transformB.lastPosition;
        const vec3 AB = _transformB.lastPosition - _transformA.lastPosition;
        const vec3 vab = va - vb;

        const f32 rab = _sphereA.radius + _sphereB.radius;
        const f32 a = dot(vab, vab);
        const f32 b = -2.0f * dot(AB, vab);
        const f32 c = dot(AB, AB) - rab * rab;

        f32 u1;
        if (dot(AB, AB) <= rab * rab)
        {
            info.hit = true;
            info.normalizedTime = 0;
        }
        if (math::get_quadratic_roots(a, b, c, info.normalizedTime, u1))
        {
            info.hit = true;
            if (info.normalizedTime > u1)
            {
                info.normalizedTime = u1;
            }
        }
        return info;
    }


    void collision_response(RigidBody& _bodyA, Transform& _transformA, vec3& _position)
    {
        vec3 x = normalized(_transformA.position - _position);

        vec3 v1 = _bodyA.velocity;
        f32 x1 = dot(x, v1);
        vec3 v1x = x * x1;
        vec3 v1y = v1 - v1x;
        f32 m1 = _bodyA.mass;

        x = x * -1.0f;
        vec3 v2{};
        f32 x2 = dot(x, v2);
        vec3 v2x = x * x2;
        f32 m2 = 100.0f;

        _bodyA.momentum = vec3(v1x*(m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y) * _bodyA.mass;
    }

    void collision_response(RigidBody& _bodyA, Transform& _transformA, RigidBody& _bodyB, Transform& _transformB)
    {
        vec3 x = normalized(_transformA.position - _transformB.position);

        vec3 v1 = _bodyA.velocity;
        f32 x1 = dot(x, v1);
        vec3 v1x = x * x1;
        vec3 v1y = v1 - v1x;
        f32 m1 = _bodyA.mass;

        x = x * -1.0f;
        vec3 v2 = _bodyB.velocity;
        f32 x2 = dot(x, v2);
        vec3 v2x = x * x2;
        vec3 v2y = v2 - v2x;
        f32 m2 = _bodyB.mass;

        _bodyA.momentum = vec3(v1x*(m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y) * _bodyA.mass;
        _bodyB.momentum = vec3(v1x*(2 * m1) / (m1 + m2) + v2x * (m2 - m1) / (m1 + m2) + v2y) * _bodyB.mass;
    }

    HitInfo detect_collision_gjk(const MeshBound& _verticesA, const Transform& _transformA, const MeshBound& _verticesB, const Transform& _transformB)
    {

        return HitInfo {};
    }

}