#pragma once


#include <vector>
#include <string>
#include <assimp/scene.h>
#include <unordered_map>

#include "gfx/gfx_mesh.h"

namespace rain
{
    struct Model
    {
        std::string path;
        std::vector<StaticMesh*> meshes;
    };

    struct ModelContainer
    {
        std::unordered_map<std::string, Model> models;
    };

    Model LoadModel(const std::string& _modelPath, const std::string& _materialDataPath, const std::string& _shaderPath);
}