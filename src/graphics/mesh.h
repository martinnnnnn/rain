#pragma once


#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <mutex>

#include "utility/incl_3d.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "graphics/material.h"

namespace rain
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textCoords;
	};

	struct Mesh
	{
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		GLuint m_vao;
		GLuint m_vbo;
		GLuint m_ebo;
	};

    struct Model
    {
        std::string path;
        Mesh* mesh;
        Material material;
        std::vector<Texture2D> textures;
    };

    std::vector<Model> SetupModel(const std::string& name, const std::string& _modelPath, const std::string& _materialDataPath, const std::string& _shaderPath);


    void InitMesh(Mesh* _mesh);
    void Draw(Model* _model);

    std::vector<Model> GetModelsFromAssimpScene(const std::string& _path);
    const aiScene* OpenAssimpScene(const std::string& _path);
    Mesh* LoadMeshData(aiMesh* _aiMesh, const aiScene* _scene);
	std::vector<Texture2D> FindTextures(const aiMesh* _aiMesh, const aiScene* _aiScene, const std::string& _directoryPath, const TextureContainer* _textureContainer);
	void GetTexturesPath(std::unordered_set<std::string>& _filePaths, const aiScene* _scene, const std::string& _directoryPath);
}