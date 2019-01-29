#pragma once


#include <vector>

#include "core/core.h"
#include "math/math.h"
#include "engine/physics/rigid_body.h"

namespace rain::engine
{
    
    struct MeshBound
    {
        std::vector<math::vec3> points;
    };

    struct HitInfo
    {
        bool hit;
        f32 normalizedTime;
        math::vec3 contactPoint;
        math::vec3 contactNormal;
    };



    HitInfo detect_collision_sphere(const math::Sphere& _sphereA, const math::Transform& _transformA, const math::Sphere& _sphereB, const math::Transform& _transformB);
    HitInfo detect_collision_sphere_plane(math::Sphere& _sphere, math::Transform& _transform, math::Plane& _plane);

    math::vec2 detect_collision_ray_sphere(const math::Ray& _ray, const math::Sphere& _sphere);

    void collision_response(RigidBody& _bodyA, math::Transform& _transformA, math::vec3& _position);
    void collision_response(RigidBody& _bodyA, math::Transform& _transformA, RigidBody& _bodyB, math::Transform& _transformB);


    HitInfo detect_collision_gjk(const MeshBound& _verticesA, const math::Transform& _transformA, const MeshBound& _verticesB, const math::Transform& _transformB);


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