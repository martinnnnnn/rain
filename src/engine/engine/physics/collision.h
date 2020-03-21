#pragma once


#include <vector>

#include "core/core.h"
#include "glm.hpp"
#include "engine/physics/rigid_body.h"

namespace rain::engine
{
    
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



    HitInfo detect_collision_sphere(const core::sphere& _sphereA, const core::transform& _transformA, const core::sphere& _sphereB, const core::transform& _transformB);
    HitInfo detect_collision_sphere_plane(core::sphere& _sphere, core::transform& _transform, core::plane& _plane);

    glm::vec2 detect_collision_ray_sphere(const core::ray& _ray, const core::sphere& _sphere);

    void collision_response(RigidBody& _bodyA, core::transform& _transformA, glm::vec3& _position);
    void collision_response(RigidBody& _bodyA, core::transform& _transformA, RigidBody& _bodyB, core::transform& _transformB);


    HitInfo detect_collision_gjk(const MeshBound& _verticesA, const core::transform& _transformA, const MeshBound& _verticesB, const core::transform& _transformB);


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