#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <fbxsdk.h>

#include "core/types.h"

namespace rain
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 binormal;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        glm::vec2 texCoords[4];
    };


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

    struct Scene
    {
        std::vector<Mesh> meshes;
    };

    void read_scene_fbx(const std::string& _path, Scene* _scene);
    void read_mesh_fbx(fbxsdk::FbxMesh* _fbxMesh, Mesh* _mesh);

    void display_textures_info(fbxsdk::FbxGeometry* pGeometry);
}