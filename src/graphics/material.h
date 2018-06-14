#pragma once


#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

#include "shader.h"
#include "ecs/component.h"

namespace rain
{
	struct Material
	{
		Shader* shader;
		std::string dataPath;
		std::vector<GLSL::Variable> shaderVariables;
	};

	Material LoadMaterial(const std::string& _dataPath, const std::string& _shaderPath);
	Material LoadMaterialData(const std::string& _dataPath, Shader* _shader);
	void WriteDefault(Material* _material);
	void SetDefaultValues(Material* material);
}