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
        Model(char* _path);
        void Draw(Shader* _shader);
        
    private:
        void loadModel(std::string _path);
        void processNode(aiNode* _node, const aiScene* _scene);
        Mesh processMesh(aiMesh* _mesh, const aiScene* _scene);
        std::vector<texture> loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName);

    private:
        std::vector<Mesh> m_meshes;
        std::string m_directory;
    };
}