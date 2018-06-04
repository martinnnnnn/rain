#include "graphics/mesh.h"

#include <fstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <mutex>
#include <future>
#include <stb_image.h>
#include <unordered_set>


#include "core/file_system.h"
#include "utility/gl_utils.h"
#include "utility/json_utils.h"
#include "utility/first_time_mcr.h"

namespace rain
{
    std::vector<Model> SetupModel(const std::string& name, const std::string& _modelPath, const std::string& _materialDataPath, const std::string& _shaderPath)
    {
        //TODO(martin) : use name instead of taking the first model in the scene
        std::vector<Model> models = GetModelsFromAssimpScene(_modelPath);

        for (size_t k = 0; k < models.size(); ++k)
        {
            InitMesh(models[k].mesh);
            models[k].material = LoadMaterial(_materialDataPath, _shaderPath);

            //TODO(martin) : not optimized - should be improved
            for (size_t i = 0; i < models[k].textures.size(); ++i)
            {
                std::string textureType = TextureUtils::Texture2DTypeToString(models[k].textures[i].type);
                for (size_t j = 0; j < models[k].material.shaderVariables.size(); ++j)
                {
                    if (models[k].material.shaderVariables[j].name.find("mat") == std::string::npos)
                        continue;
                    if (models[k].material.shaderVariables[j].name.find(textureType) != std::string::npos)
                    {
                        models[k].material.shaderVariables[j].textureId = models[k].textures[i].id;
                        break;
                    }
                }
            }
        }
        return models;
    }


	//TODO(martin) : error handling
    std::vector<Model> GetModelsFromAssimpScene(const std::string& _path)
    {
        std::vector<Model> models;
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return models;
        }

        //const aiScene* scene = OpenAssimpScene(_path);

        models.reserve(scene->mNumMeshes);
		std::string directoryPath = File::GetDirectory(_path);
		std::unordered_set<std::string> filePaths;
		GetTexturesPath(filePaths, scene, directoryPath);

		TextureContainer textureContainer = {};
		LoadTexturesAsync(&textureContainer, filePaths);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
        {

			aiMesh* aimesh = scene->mMeshes[i];

			Model model{};
			model.path = _path;

			model.mesh = LoadMeshData(aimesh, scene);

			model.textures = FindTextures(aimesh, scene, directoryPath, &textureContainer);
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
            Vertex vert;
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

	std::vector<Texture2D> FindTextures(const aiMesh* _aiMesh, const aiScene* _aiScene, const std::string& _directoryPath, const TextureContainer* _textureContainer)
	{
		std::vector<Texture2D> textures;

		if (_aiMesh->mMaterialIndex < 0)
			return textures;

		aiMaterial* material = _aiScene->mMaterials[_aiMesh->mMaterialIndex];

		for (aiTextureType i = (aiTextureType)0x0; i < AI_TEXTURE_TYPE_MAX;)
		{
			i = (aiTextureType)((int)i + 1);
			unsigned int textureCount = material->GetTextureCount(i);

			for (unsigned int j = 0; j < textureCount; ++j)
			{
				aiString str;
				material->GetTexture(i, j, &str);

				Texture2D tex = FindTexture(_textureContainer, File::GetFileName(str.C_Str()));
				tex.type = Texture2DType(i);
				textures.push_back(tex);
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
        glBufferData(GL_ARRAY_BUFFER, _mesh->vertices.size() * sizeof(Vertex), &_mesh->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indices.size() * sizeof(unsigned int), &_mesh->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCoords));

        glBindVertexArray(0);
    }

    void Draw(Model* _model)
    {
        _model->material.shader->use();

        int textIndex = 0;
        for (size_t i = 0; i < _model->material.shaderVariables.size(); ++i)
        {
            GLSL::Variable* variable = &_model->material.shaderVariables[i];
            for (size_t j = 0; j < _model->textures.size(); ++j)
            {
                if (_model->textures[j].id == variable->textureId)
                {
                    glActiveTexture(GL_TEXTURE0 + textIndex);
                    textIndex++;
                    _model->material.shader->setParameter(variable->name, (int)j);
                    glBindTexture(GL_TEXTURE_2D, _model->textures[j].id);
                    continue;
                }
            }
        }

        glBindVertexArray(_model->mesh->m_vao);
        glDrawElements(GL_TRIANGLES, _model->mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

	void GetTexturesPath(std::unordered_set<std::string>& _filePaths, const aiScene* _scene, const std::string& _directoryPath)
	{
		for (unsigned int i = 0; i < _scene->mNumMeshes; ++i)
		{
			aiMesh* mesh = _scene->mMeshes[i];
			if (mesh->mMaterialIndex < 0)
				continue;
			aiMaterial* material = _scene->mMaterials[mesh->mMaterialIndex];

			for (aiTextureType texType = (aiTextureType)0x0; texType < AI_TEXTURE_TYPE_MAX;)
			{
				texType = (aiTextureType)((int)texType + 1);
				unsigned int textureCount = material->GetTextureCount(texType);

				for (unsigned int j = 0; j < textureCount; ++j)
				{
					aiString str;
					material->GetTexture(texType, j, &str);
					_filePaths.emplace(_directoryPath + str.C_Str());
				}
			}
		}
	}
}