#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "core/types.h"
#include "physics/rigid_body.h"

namespace rain
{

    struct Ray
    {
        glm::vec3 origin;
        glm::vec3 direction;
    };




    struct Sphere
    {
        Sphere()
            : Sphere(0.0f, glm::vec3(0.0f, 0.0f, 0.0f))
        {}

        Sphere(float _radius, glm::vec3 _offset)
            : radius(_radius)
            , offset(_offset)
        {}

        // offset from the center of the object - which must have a transform
        f32 radius;
        glm::vec3 offset;
    };

    struct Plane
    {
        Plane()
            : n(glm::vec3(0.0))
            , D(-glm::dot(n, glm::vec3(0.0)))
        {}

        Plane(const glm::vec3& _p0, const glm::vec3& _n)
            : n(_n)
            , D(-glm::dot(n,_p0))
        {
        }

        Plane(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2)
            : n(glm::cross((_p1 - _p0), glm::normalize(_p2 - _p0)))
            , D(-glm::dot(n, _p0))
        {
        }
        
        const f32 distance_to_point(const glm::vec3& _p) const
        {
            return glm::dot(n, _p) + D;
        }

        glm::vec3 project(glm::vec3 _point)
        {
            return _point - (distance_to_point(_point) * n);
        }

        glm::vec3 n;
        f32 D;
    };

    struct MeshBound
    {
        std::vector<glm::vec3> points;
    };

    struct Quad
    {

    };

    struct Box
    {
    };

    struct HitInfo
    {
        bool hit;
        f32 normalizedTime;
        glm::vec3 contactPoint;
        glm::vec3 contactNormal;
    };


    bool get_quadratic_roots(const f32 _a, const f32 _b, const f32 _c, f32& _r1, f32& _r2);

    HitInfo detect_collision_sphere(const Sphere& _sphereA, const Transform& _transformA, const Sphere& _sphereB, const Transform& _transformB);
    HitInfo detect_collision_sphere_plane( Sphere& _sphere, Transform& _transform, Plane& _plane);


    glm::vec2 detect_collision_ray_sphere(const Ray& _ray, const Sphere& _sphere);



    void collision_response(RigidBody& _bodyA, Transform& _transformA, glm::vec3& _position);
    void collision_response(RigidBody& _bodyA, Transform& _transformA, RigidBody& _bodyB, Transform& _transformB);


    HitInfo detect_collision_gjk(const MeshBound& _verticesA, const Transform& _transformA, const MeshBound& _verticesB, const Transform& _transformB);
}