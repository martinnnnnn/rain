#include "graphics/mesh.h"

#include <fstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <nlohmann/json.hpp>

#include "core/file_system.h"
#include "utility/gl_utils.h"
#include "utility/json_utils.h"


namespace rain
{
    Model SetupModel(const std::string& name, const std::string& _modelPath, const std::string& _materialDataPath, const std::string& _shaderPath)
    {
        //TODO(martin) : use name instead of taking the first model in the scene
        Model model = GetModelsFromAssimpScene(_modelPath)[0];
        InitMesh(model.mesh);
        model.material = LoadMaterial(_materialDataPath, _shaderPath);

        //TODO(martin) : not optimized - should be improved
        for (size_t j = 0; j < model.textures.size(); ++j)
        {
            for (auto shaderVariable : model.material.shaderVariables)
            {
                std::string textureType = TextureUtils::Texture2DTypeToString(model.textures[j].type);
                if (shaderVariable.name.find(textureType) != std::string::npos)
                {
                    shaderVariable.textureName = model.textures[j].path;
                    break;
                }
            }
        }
        return model;
    }




    Material LoadMaterial(const std::string& _dataPath, const std::string& _shaderPath)
    {
        Material material;

        material.dataPath = _dataPath;
        std::string vertexpath = _shaderPath + ".vs";
        std::string fragmentpath = _shaderPath + ".fs";

        material.shader = new Shader();
        material.shader->init(vertexpath, fragmentpath);
        material.shader->use();
        material.shaderVariables = material.shader->GetGLSLVariablesSimple();

        WriteDefault(&material);
        SetDefaultValues(&material);
        
        return material;
    }

    void WriteDefault(Material* _material)
    {
        std::ofstream outfile(_material->dataPath);

        nlohmann::json jsonVariables;

        for (size_t i = 0; i < _material->shaderVariables.size(); ++i)
        {
            nlohmann::json json;
            json["glsl_type"] = GLUtils::GLTypeToString(_material->shaderVariables[i].glslType);
            switch (_material->shaderVariables[i].glslType)
            {
            case GL_FLOAT:
                json["value"] = 0;
                break;
            case GL_FLOAT_VEC2:
                json["value"] = glm::vec2(0);
                break;
            case GL_FLOAT_VEC3:
                json["value"] = glm::vec3(0);
                break;
            case GL_FLOAT_MAT4:
                json["value"] = glm::mat4(0);
                break;
            case GL_SAMPLER_2D:
                json["value"] = 0;
                break;
            case GL_SAMPLER_CUBE:
                json["value"] = 0;
                break;
            default:
                json["value"] = 0;
                break;
            }
            jsonVariables[_material->shaderVariables[i].name] = json;
        }
        outfile << jsonVariables.dump(4);
        outfile.close();
    }

    void SetDefaultValues(Material* _material)
    {
        //TODO(martin) : get rid of std for file reading
        std::ifstream file(_material->dataPath);
        if (file.fail())
        {
            //TODO(martin) : handle error better
            return;
        }

        nlohmann::json jsonVariables;
        file >> jsonVariables;

        for (size_t i = 0; i < _material->shaderVariables.size(); ++i)
        {
            if (_material->shaderVariables[i].variableType == GLSL::Type::UNIFORM)
            {
                nlohmann::json obj = jsonVariables[_material->shaderVariables[i].name];

                GLenum glslType = GLUtils::StringToGLType(obj["glsl_type"].get<std::string>());
                switch (glslType)
                {
                case GL_FLOAT:
                {
                    _material->shaderVariables[i].value.m_float = obj["value"].get<float>();
                } break;

                case GL_FLOAT_VEC2:
                {
                    _material->shaderVariables[i].value.m_vec2 = obj["value"].get<glm::vec2>();
                } break;

                case GL_FLOAT_VEC3:
                {
                    _material->shaderVariables[i].value.m_vec3 = obj["value"].get<glm::vec3>();
                } break;

                case GL_FLOAT_MAT4:
                {
                    _material->shaderVariables[i].value.m_mat4 = obj["value"].get<glm::mat4>();
                } break;

                case GL_SAMPLER_2D:
                {
                    _material->shaderVariables[i].value.m_int = obj["value"].get<int>();
                } break;

                case GL_SAMPLER_CUBE:
                {
                    _material->shaderVariables[i].value.m_int = obj["value"].get<int>();
                } break;

                default:
                {
                    _material->shaderVariables[i].value.m_int = obj["value"].get<int>();
                }
                }
            }
        }
    }

    Material LoadMaterialData(const std::string& _dataPath, Shader* _shader)
    {
        if (!_shader)
        {
            //TODO(martin) : handle error
            return Material{};
        }
        Material material;

        material.dataPath = _dataPath;
        material.shader = _shader;
        material.shader->use();
        material.shaderVariables = material.shader->GetGLSLVariablesSimple();

        SetDefaultValues(&material);

        return material;
    }

    std::vector<Model> GetModelsFromAssimpScene(const std::string& _path)
    {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        }



        //TODO(martin) : error handling
        // loading scene
        //const aiScene* scene = OpenAssimpScene(_path);

        // reserving space for models
        std::vector<Model> models;
        models.reserve(scene->mNumMeshes);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
        {
            aiMesh* aimesh = scene->mMeshes[i];

            Model model {};
            model.path = _path;
            model.mesh = LoadMeshData(aimesh, scene);
            model.textures = LoadMeshTextures(aimesh, scene, File::GetDirectory(_path));
            models.push_back(model);
        }
        return models;
    }

    const aiScene* OpenAssimpScene(const std::string& _path)
    {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return nullptr;
        }
        return scene;
    }

    Mesh* LoadMeshData(aiMesh* _aiMesh, const aiScene* _scene)
    {
        Mesh* mesh = new Mesh();

        // setting vertices
        mesh->vertices.reserve(_aiMesh->mNumVertices);
        for (unsigned int i = 0; i < _aiMesh->mNumVertices; ++i)
        {
            vertex vert;
            vert.position.x = _aiMesh->mVertices[i].x;
            vert.position.y = _aiMesh->mVertices[i].y;
            vert.position.z = _aiMesh->mVertices[i].z;

            vert.normal.x = _aiMesh->mNormals[i].x;
            vert.normal.y = _aiMesh->mNormals[i].y;
            vert.normal.z = _aiMesh->mNormals[i].z;

            vert.textCoords = glm::vec2(0.0, 0.0);
            if (_aiMesh->mTextureCoords[0])
            {
                vert.textCoords.x = _aiMesh->mTextureCoords[0][i].x;
                vert.textCoords.y = _aiMesh->mTextureCoords[0][i].y;
            }

            mesh->vertices.push_back(vert);
        }

        // setting indices
        mesh->indices.reserve(_aiMesh->mNumFaces);
        for (unsigned int i = 0; i < _aiMesh->mNumFaces; ++i)
        {
            aiFace face = _aiMesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                mesh->indices.push_back(face.mIndices[j]);
            }
        }
        return mesh;
    }


    std::vector<Texture2D> LoadMeshTextures(aiMesh* _aiMesh, const aiScene* _aiScene, const std::string& _directoryPath)
    {
        std::vector<Texture2D> textures;

        if (_aiMesh->mMaterialIndex >= 0)
        {
            textures.reserve(_aiMesh->mMaterialIndex);
            aiMaterial* material = _aiScene->mMaterials[_aiMesh->mMaterialIndex];

            for (aiTextureType i = (aiTextureType)0x0; i < AI_TEXTURE_TYPE_MAX;)
            {
                i = (aiTextureType)((int)i + 1);
                unsigned int textureCount = material->GetTextureCount(i);
                for (unsigned int j = 0; j < textureCount; ++j)
                {
                    aiString str;
                    material->GetTexture(i, j, &str);
                    Texture2D tex = LoadTexture2D(_directoryPath + str.C_Str(), (Texture2DType)i);
                    textures.push_back(tex);
                }
            }
        }
        return textures;
    }


    void InitMesh(Mesh* _mesh)
    {
        glGenVertexArrays(1, &_mesh->m_vao);
        glGenBuffers(1, &_mesh->m_vbo);
        glGenBuffers(1, &_mesh->m_ebo);

        glBindVertexArray(_mesh->m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, _mesh->m_vbo);
        glBufferData(GL_ARRAY_BUFFER, _mesh->vertices.size() * sizeof(vertex), &_mesh->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indices.size() * sizeof(unsigned int), &_mesh->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, textCoords));

        glBindVertexArray(0);
    }

    void Draw(Model* _model)
    {
        // TODO(martin) : change this draw method once material struct is setup
        _model->material.shader->use();

        for (size_t i = 0; i < _model->material.shaderVariables.size(); ++i)
        {
            GLSL::Variable variable = _model->material.shaderVariables[i];
            for (auto texture : _model->textures)
            {
                if (texture.path == variable.textureName)
                {
                    glActiveTexture(GL_TEXTURE0 + i);
                    _model->material.shader->setParameter(variable.name, (int)i);
                    glBindTexture(GL_TEXTURE_2D, texture.id);
                    continue;
                }
            }
        }

        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(_model->mesh->m_vao);
        glDrawElements(GL_TRIANGLES, _model->mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}




//std::vector<aiMesh*> GetMeshes(aiNode* _node, const aiScene* _scene)
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