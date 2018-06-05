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
    
	struct StaticMeshData
	{
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		GLuint m_vao;
		GLuint m_vbo;
		GLuint m_ebo;
	};

    struct StaticMesh
    {
        StaticMeshData* mesh;
        Material material;
        std::vector<Texture2D> textures;
    };

    void InitMesh(StaticMeshData* _mesh);
    void Draw(StaticMesh* _mesh);

    std::vector<StaticMesh*> GetMeshesFromAssimpScene(const std::string& _path);
    StaticMeshData* LoadMeshData(aiMesh* _aiMesh, const aiScene* _scene);
	std::vector<Texture2D> FindTextures(const aiMesh* _aiMesh, const aiScene* _aiScene, const std::string& _directoryPath, const TextureContainer* _textureContainer);
	void GetTexturesPath(std::unordered_set<std::string>& _filePaths, const aiScene* _scene, const std::string& _directoryPath);
}