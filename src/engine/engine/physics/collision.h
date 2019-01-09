#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "core/core.h"
#include "engine/physics/rigid_body.h"

namespace rain::engine
{
    using core::math::Plane;
    using core::math::Sphere;
    using core::math::Ray;

    struct MeshBound
    {
        std::vector<glm::vec3> points;
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