#pragma once


#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/scene.h>

#include "utility/incl_3d.h"
#include "graphics/texture.h"
#include "shader.h"

namespace rain
{
	class Shader;

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

    struct Material
    {
        Shader* shader;
        std::string dataPath;
        std::vector<GLSL::Variable> shaderVariables;
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

    
    Material LoadMaterial(const std::string& _dataPath, const std::string& _shaderPath);
    Material LoadMaterialData(const std::string& _dataPath, Shader* _shader);
    void WriteDefault(Material* _material);
    void SetDefaultValues(Material* material);
    
    std::vector<Model> GetModelsFromAssimpScene(const std::string& _path);
    const aiScene* OpenAssimpScene(const std::string& _path);
    Mesh* LoadMeshData(aiMesh* _aiMesh, const aiScene* _scene);
    std::vector<Texture2D> LoadMeshTextures(aiMesh* _aiMesh, const aiScene* _aiScene, const std::string& _directoryPath);
}