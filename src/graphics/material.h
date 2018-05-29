#pragma once


#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

#include "shader.h"
#include "core/component.h"

namespace rain
{
    class Texture;

    class Material : public Component
    {
    public:
        Material() {}
        Material(const std::string& _shaderPath, std::vector<Texture*> _textures, bool _eraseDefault = true);

		void Init(const std::string& _shaderPath, std::vector<Texture*> _textures, bool _eraseDefault = true);
        
	private:
		void writeShaderDefaultValueFile(const std::string& _datafilePath);
		void readShaderDefaultValueFile(std::ifstream& _inputFile);

    private:
        Shader* m_shader;
        std::string m_shaderPath;
        std::unordered_map<std::string, std::vector<GLSL::Variable>> m_shaderVariables;
        std::vector<Texture*> m_textures;

    };
}