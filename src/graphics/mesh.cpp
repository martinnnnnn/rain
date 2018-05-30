#include "graphics/mesh.h"

#include <fstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <nlohmann/json.hpp>

#include "core/file_system.h"
#include "shader.h"
#include "utility/gl_utils.h"


namespace rain
{

    Material LoadMaterial(const std::string& _dataPath, const std::string& _shaderPath)
    {
        Material material;

        material.shaderPath = _shaderPath;
        material.dataPath = _dataPath;
        std::string vertexpath = material.shaderPath + ".vs";
        std::string fragmentpath = material.shaderPath + ".fs";

        material.shader = new Shader();
        material.shader->init(vertexpath, fragmentpath);
        material.shader->use();
        material.shaderVariables = material.shader->GetGLSLVariablesSimple();

        SetDefaultValues(&material);
        
        return material;
    }

    void SetDefaultValues(Material* material)
    {
        //TODO(martin) : get rid of std for file reading
        std::ifstream file(material->dataPath);
        if (file.fail())
        {
            //TODO(martin) : handle error better
            return;
        }

        nlohmann::json jsonVariables;
        file >> jsonVariables;

        for (size_t i = 0; i < material->shaderVariables.size(); ++i)
        {
            if (material->shaderVariables[i].variableType == GLSL::Type::UNIFORM)
            {
                nlohmann::json obj = jsonVariables[material->shaderVariables[i].name];

                GLenum glslType = GLUtils::StringToGLType(obj["glsl_type"].get<std::string>());
                switch (glslType)
                {
                case GL_FLOAT:
                {
                    material->shaderVariables[i].value.m_float = obj["value"].get<float>();
                } break;

                case GL_FLOAT_VEC2:
                {
                    material->shaderVariables[i].value.m_vec2 = obj["value"].get<glm::vec2>();
                } break;

                case GL_FLOAT_VEC3:
                {
                    material->shaderVariables[i].value.m_vec3 = obj["value"].get<glm::vec3>();
                } break;

                case GL_FLOAT_MAT4:
                {
                    material->shaderVariables[i].value.m_mat4 = obj["value"].get<glm::mat4>();
                } break;

                case GL_SAMPLER_2D:
                {
                    material->shaderVariables[i].value.m_int = obj["value"].get<int>();
                } break;

                case GL_SAMPLER_CUBE:
                {
                    material->shaderVariables[i].value.m_int = obj["value"].get<int>();
                } break;

                default:
                {
                    material->shaderVariables[i].value.m_int = obj["value"].get<int>();
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

        material.shaderPath = "unknown";
        material.dataPath = _dataPath;
        material.shader = _shader;
        material.shader->use();
        material.shaderVariables = material.shader->GetGLSLVariablesSimple();

        SetDefaultValues(&material);

        return material;
    }

    std::vector<Model> GetModelsFromAssimpScene(const std::string& _path)
    {
        // loading scene
        const aiScene* scene = OpenAssimpScene(_path);
        aiNode* node = scene->mRootNode;

        // reserving space for models
        std::vector<Model> models;
        models.reserve(scene->mNumMeshes);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
        {
            aiMesh* aimesh = scene->mMeshes[i];

            Model model {};
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
            return;
        }

        return scene;
    }

    std::vector<aiMesh*> GetMeshes(aiNode* _node, const aiScene* _scene, Mesh* parent)
    {

    }


    Mesh* LoadMeshData(aiMesh* _aiMesh, const aiScene* _scene)
    {
        Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));

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
                    Texture2D tex;
                    LoadTexture2D(_directoryPath + str.C_Str(), (Texture2DType)i);
                    textures.push_back(tex);
                }
            }
        }
        return textures;
    }


    Mesh* InitMesh(std::vector<vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture2D> _textures)
    {
        //CHECK(martin) : need to check is this initialization actually work
        Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
        mesh->vertices = _vertices;
        mesh->indices = _indices;
        mesh->textures = _textures;

        glGenVertexArrays(1, &mesh->m_vao);
        glGenBuffers(1, &mesh->m_vbo);
        glGenBuffers(1, &mesh->m_ebo);

        glBindVertexArray(mesh->m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(vertex), &mesh->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, textCoords));

        glBindVertexArray(0);

        return mesh;
    }

    void DrawMesh(Mesh* _mesh, Shader* _shader)
    {
        // TODO(martin) : change this draw method once material struct is setup
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for (unsigned int i = 0; i < _mesh->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = _mesh->textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            _shader->setParameter(("material." + name + number).c_str(), (int)i);
            glBindTexture(GL_TEXTURE_2D, _mesh->textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(_mesh->m_vao);
        glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
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