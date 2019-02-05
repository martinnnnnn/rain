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



    HitInfo detect_collision_sphere(const math::sphere& _sphereA, const math::transform& _transformA, const math::sphere& _sphereB, const math::transform& _transformB);
    HitInfo detect_collision_sphere_plane(math::sphere& _sphere, math::transform& _transform, math::plane& _plane);

    math::vec2 detect_collision_ray_sphere(const math::ray& _ray, const math::sphere& _sphere);

    void collision_response(RigidBody& _bodyA, math::transform& _transformA, math::vec3& _position);
    void collision_response(RigidBody& _bodyA, math::transform& _transformA, RigidBody& _bodyB, math::transform& _transformB);


    HitInfo detect_collision_gjk(const MeshBound& _verticesA, const math::transform& _transformA, const MeshBound& _verticesB, const math::transform& _transformB);


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