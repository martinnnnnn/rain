#pragma once


#include <vector>

#include "core/core.h"
#include "math/math.h"
#include "engine/physics/rigid_body.h"

namespace rain::engine
{
    using math::Plane;
    using math::Transform;
    using math::Sphere;
    using math::Ray;
    using math::vec2;
    using math::vec3;

    struct MeshBound
    {
        std::vector<vec3> points;
    };

    struct HitInfo
    {
        bool hit;
        f32 normalizedTime;
        vec3 contactPoint;
        vec3 contactNormal;
    };



    HitInfo detect_collision_sphere(const Sphere& _sphereA, const Transform& _transformA, const Sphere& _sphereB, const Transform& _transformB);
    HitInfo detect_collision_sphere_plane( Sphere& _sphere, Transform& _transform, Plane& _plane);

    vec2 detect_collision_ray_sphere(const Ray& _ray, const Sphere& _sphere);

    void collision_response(RigidBody& _bodyA, Transform& _transformA, vec3& _position);
    void collision_response(RigidBody& _bodyA, Transform& _transformA, RigidBody& _bodyB, Transform& _transformB);


    HitInfo detect_collision_gjk(const MeshBound& _verticesA, const Transform& _transformA, const MeshBound& _verticesB, const Transform& _transformB);


    //namespace convex_hull
    //{
    //    get_initial_tetrahedron(const )
    //    {
    //        const size_t vertexCount = m_vertexData.size();

    //        // If we have at most 4 points, just return a degenerate tetrahedron:
    //        if (vertexCount <= 4) {
    //            IndexType v[4] = { 0,std::min((size_t)1,vertexCount - 1),std::min((size_t)2,vertexCount - 1),std::min((size_t)3,vertexCount - 1) };
    //            const Vector3<T> N = mathutils::getTriangleNormal(m_vertexData[v[0]], m_vertexData[v[1]], m_vertexData[v[2]]);
    //            const Plane<T> trianglePlane(N, m_vertexData[v[0]]);
    //            if (trianglePlane.isPointOnPositiveSide(m_vertexData[v[3]])) {
    //                std::swap(v[0], v[1]);
    //            }
    //            return MeshBuilder<T>(v[0], v[1], v[2], v[3]);
    //        }
    //}

}