#pragma once


#include <vector>
#include <string>
#include <assimp/scene.h>

#include "graphics/mesh.h"


namespace rain
{
    struct Model
    {
        std::string path;
        std::vector<StaticMesh*> meshes;
    };

    Model SetupModel(const std::string& name, const std::string& _modelPath, const std::string& _materialDataPath, const std::string& _shaderPath);
}