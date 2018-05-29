#pragma once


#include <vector>
#include <string>

#include <assimp/scene.h>


#include "graphics/mesh.h"


namespace rain
{
    class Shader;

    class Model
    {
    public:
        Model(const std::string& _path);
        void Draw(Shader* _shader);

        std::vector<Mesh> m_meshes;

    private:
        void loadModel(const std::string& _path);
        void processNode(aiNode* _node, const aiScene* _scene);
        Mesh processMesh(aiMesh* _mesh, const aiScene* _scene);
        std::vector<Texture2D> loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName);

        std::string m_path;
    };
}