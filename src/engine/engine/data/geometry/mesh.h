#pragma once

#include <vector>
#include <fbxsdk.h>

#include "core/core.h"
#include "math/math.h"
#include "engine/data/data_handle.h"

namespace rain::engine
{

    struct Vertex
    {
        math::vec3 position;
        math::vec3 normal;
        math::vec3 binormal;
        math::vec3 tangent;
        math::vec3 bitangent;
        math::vec2 texCoords[4];
    };

    bool operator==(const Vertex& _a, const Vertex& _b);

    bool operator!=(const Vertex& _a, const Vertex& _b);


    struct Mesh
    {
        Mesh()
            : vao(0)
            , vbo(0)
            , ebo(0)
        {}
        std::vector<Vertex> vertices;
        std::vector<u32> indices;
        u32 vao;
        u32 vbo;
        u32 ebo;

        void load(const std::string& _path);
    };

    struct Model
    {
        core::file_path path;
        handle<Mesh> const * mesh;
    };

    void read_mesh_fbx(fbxsdk::FbxMesh* _fbxMesh, Mesh* _mesh);
    void display_metadata_connection(FbxObject* pObject);
    void display_link(FbxGeometry* pGeometry);
    void display_skeleton(FbxNode* pNode, std::string tab);


    //void compute_convex_hull(const std::vector<Vertex>& _input)
    //{
    //    std::vector<Vertex> hull;
    //}

}