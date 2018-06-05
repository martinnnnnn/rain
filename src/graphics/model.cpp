#include "graphics/model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "core/file_system.h"


namespace rain
{
    Model LoadModel(const std::string& _modelPath, const std::string& _materialDataPath, const std::string& _shaderPath)
    {
        Model model = {};
        model.path = _modelPath;
        model.meshes = GetMeshesFromAssimpScene(_modelPath);

        //std::vector<StaticMesh> models = GetModelsFromAssimpScene(_modelPath);

        for (size_t k = 0; k < model.meshes.size(); ++k)
        {
            InitMesh(model.meshes[k]->mesh);
            model.meshes[k]->material = LoadMaterial(_materialDataPath, _shaderPath);

            //TODO(martin) : not optimized - should be improved
            for (size_t i = 0; i < model.meshes[k]->textures.size(); ++i)
            {
                std::string textureType = TextureUtils::Texture2DTypeToString(model.meshes[k]->textures[i].type);
                for (size_t j = 0; j < model.meshes[k]->material.shaderVariables.size(); ++j)
                {
                    if (model.meshes[k]->material.shaderVariables[j].name.find("mat") == std::string::npos)
                        continue;
                    if (model.meshes[k]->material.shaderVariables[j].name.find(textureType) != std::string::npos)
                    {
                        model.meshes[k]->material.shaderVariables[j].textureId = model.meshes[k]->textures[i].id;
                        break;
                    }
                }
            }
        }
        return model;
    }
}