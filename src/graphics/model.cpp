#include "graphics/model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "core/file_system.h"


namespace rain
{

    //std::vector<Mesh> loadMeshes(const std::string& _path)
    //{
    //    Assimp::Importer import;
    //    const aiScene *scene = import.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    //    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    //    {
    //        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    //        return;
    //    }

    //    return processNode(scene->mRootNode, scene);
    //}

    //std::vector<Mesh> processNode(aiNode* _node, const aiScene* _scene)
    //{
    //    std::vector<Mesh> meshes;
    //    for (unsigned int i = 0; i < _node->mNumMeshes; i++)
    //    {
    //        aiMesh *mesh = _scene->mMeshes[_node->mMeshes[i]];
    //        meshes.push_back(processMesh(mesh, _scene));
    //    }

    //    for (unsigned int i = 0; i < _node->mNumChildren; i++)
    //    {
    //        std::vector<Mesh> childMeshes = processNode(_node->mChildren[i], _scene);
    //        meshes.insert(meshes.end(), childMeshes.begin(), childMeshes.end());
    //    }
    //    return meshes;
    //}

    //Mesh processMesh(aiMesh* _mesh, const aiScene* _scene)
    //{
    //    std::vector<vertex> vertices;
    //    std::vector<unsigned int> indices;
    //    std::vector<Texture2D> textures;

    //    for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
    //    {
    //        vertex vert;
    //        vert.position.x = _mesh->mVertices[i].x;
    //        vert.position.y = _mesh->mVertices[i].y;
    //        vert.position.z = _mesh->mVertices[i].z;

    //        vert.normal.x = _mesh->mNormals[i].x;
    //        vert.normal.y = _mesh->mNormals[i].y;
    //        vert.normal.z = _mesh->mNormals[i].z;

    //        vert.textCoords = glm::vec2(0.0, 0.0);
    //        if (_mesh->mTextureCoords[0])
    //        {
    //            vert.textCoords.x = _mesh->mTextureCoords[0][i].x;
    //            vert.textCoords.y = _mesh->mTextureCoords[0][i].y;
    //        }

    //        vertices.push_back(vert);
    //    }

    //    for (unsigned int i = 0; i < _mesh->mNumFaces; ++i)
    //    {
    //        aiFace face = _mesh->mFaces[i];
    //        for (unsigned int j = 0; j < face.mNumIndices; ++j)
    //        {
    //            indices.push_back(face.mIndices[j]);
    //        }
    //    }

    //    if (_mesh->mMaterialIndex >= 0)
    //    {
    //        aiMaterial *material = _scene->mMaterials[_mesh->mMaterialIndex];

    //        std::vector<Texture2D> textures;

    //        for (aiTextureType i = (aiTextureType)0x0; i < AI_TEXTURE_TYPE_MAX;)
    //        {
    //            i = (aiTextureType)((int)i + 1);
    //            //i = (aiTextureType)((int)i + 1);
    //            unsigned int textureCount = material->GetTextureCount(i);
    //            for (unsigned int j = 0; j < textureCount; ++j)
    //            {
    //                aiString str;
    //                material->GetTexture(i, j, &str);
    //                Texture2D tex;
    //                tex.Load(File::GetDirectory(m_path), str.C_Str(), (Texture2DType)i);
    //                std::cout << str.C_Str() << " : " << TextureUtils::Texture2DTypeToString((Texture2DType)i) << std::endl;
    //                textures.push_back(tex);
    //            }
    //        }

    //        

    //        //std::vector<Texture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    //        //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    //        //std::vector<Texture2D> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    //        //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //    }

    //    return Mesh(vertices, indices, textures);
    //}
}