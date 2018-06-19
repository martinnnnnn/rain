#include "gfx/gfx_material.h"

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "utility/gl_utils.h"
#include "rain.h"
#include "gfx/gfx_shader.h"
#include "utility/incl_3d.h"
#include "utility/json_utils.h"

//using namespace nlohmann;

namespace rain
{
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
            _material = nullptr;
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
}
