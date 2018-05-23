#include "material.h"


#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "utility/gl_utils.h"
#include "rain.h"
#include "graphics/shader.h"
#include "utility/incl_3d.h"
#include "utility/json_utils.h"

//using namespace nlohmann;

namespace rain
{
    Material::Material(const std::string& _shaderPath, bool _createDefaultIfMissing) :
        m_shaderPath(_shaderPath)
    {
        Init(_shaderPath, _createDefaultIfMissing);
    }

    void Material::Init(const std::string& _shaderPath, bool _eraseDefault)
    {
        m_shaderPath = _shaderPath;
        std::string vertexpath = m_shaderPath + ".vs";
        std::string fragmentpath = m_shaderPath + ".fs";
        std::string datapath = m_shaderPath + ".json";

        m_shader = new Shader();
        m_shader->init(vertexpath, fragmentpath);
        m_shader->use();

        m_shaderVariables = m_shader->GetGLSLVariables();

		std::ifstream file(datapath);

		if (file.fail())
		{
			writeShaderDefaultValueFile(datapath);
		}
		else
		{
			if (_eraseDefault)
			{
				writeShaderDefaultValueFile(datapath);
			}
			else
			{
				readShaderDefaultValueFile(file);
			}
		}
		
	}


	void Material::writeShaderDefaultValueFile(const std::string& _datafilePath)
	{
		std::ofstream outfile(_datafilePath);

		nlohmann::json variables;
		for (size_t i = 0; i < m_shaderVariables.size(); ++i)
		{
            if (m_shaderVariables[i].variableType == GLSLVariableType::UNIFORM)
            {
			    nlohmann::json json;
			    json["glsl_type"] = GLUtils::GLTypeToString(m_shaderVariables[i].glslType);
			    switch (m_shaderVariables[i].glslType)
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
			    variables[m_shaderVariables[i].name] = json;
            }
		}

		outfile << variables.dump(4, true);
		outfile.close();
	}

	void Material::readShaderDefaultValueFile(std::ifstream& _inputFile)
	{
        nlohmann::json variables;
        _inputFile >> variables;

        for (size_t i = 0; i < m_shaderVariables.size(); ++i)
        {
            if (m_shaderVariables[i].variableType == GLSLVariableType::UNIFORM)
            {
                nlohmann::json obj = variables[m_shaderVariables[i].name];

                std::string typestr = obj["glsl_type"].get<std::string>();
                GLenum value = GLUtils::StringToGLType(typestr);

                switch (value)
                {
                case GL_FLOAT:
                    m_shaderVariables[i].value.m_float = obj["value"].get<float>();
                    break;
                case GL_FLOAT_VEC2:
                    m_shaderVariables[i].value.m_vec2 = obj["value"].get<glm::vec2>();
                    break;
                case GL_FLOAT_VEC3:
                    m_shaderVariables[i].value.m_vec3 = obj["value"].get<glm::vec3>();
                    break;
                case GL_FLOAT_MAT4:
                    m_shaderVariables[i].value.m_mat4 = obj["value"].get<glm::mat4>();
                    break;
                case GL_SAMPLER_2D:
                    m_shaderVariables[i].value.m_int = obj["value"].get<int>();
                    break;
                case GL_SAMPLER_CUBE:
                    m_shaderVariables[i].value.m_int = obj["value"].get<int>();
                    break;
                default:
                    m_shaderVariables[i].value.m_int = obj["value"].get<int>();
                    break;
                }

                // printing each value
                switch (m_shaderVariables[i].glslType)
                {
                case GL_FLOAT:
                    std::cout << std::setw(20) << std::left << m_shaderVariables[i].name << "-> m_float:" << m_shaderVariables[i].value.m_float << std::endl;
                    break;
                case GL_FLOAT_VEC2:
                    std::cout << std::setw(20) << std::left << m_shaderVariables[i].name << "-> m_vec2:(" << m_shaderVariables[i].value.m_vec2.x << "," << m_shaderVariables[i].value.m_vec2.y << ")" << std::endl;
                    break;
                case GL_FLOAT_VEC3:
                    std::cout << std::setw(20) << std::left << m_shaderVariables[i].name << "-> m_vec3:(" << m_shaderVariables[i].value.m_vec3.x << "," << m_shaderVariables[i].value.m_vec3.y << "," << m_shaderVariables[i].value.m_vec3.z << ")" << std::endl;
                    break;
                case GL_FLOAT_MAT4:
                    std::cout << std::setw(20) << std::left << m_shaderVariables[i].name << "-> m_mat4:" << m_shaderVariables[i].value.m_mat4[0][0] << std::endl;
                    break;
                case GL_SAMPLER_2D:
                    std::cout << std::setw(20) << std::left << m_shaderVariables[i].name << "-> m_int:" << m_shaderVariables[i].value.m_int << std::endl;
                    break;
                case GL_SAMPLER_CUBE:
                    std::cout << std::setw(20) << std::left << m_shaderVariables[i].name << "-> m_int:" << m_shaderVariables[i].value.m_int << std::endl;
                    break;
                default:
                    std::cout << std::setw(20) << std::left << m_shaderVariables[i].name << "-> m_int:" << m_shaderVariables[i].value.m_int << std::endl;
                    break;
                }
            }
        }
	}
}
