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
			// file cannot be open : has to be created
			std::cout << "creating file" << std::endl;
			writeShaderDefaultValueFile(datapath);
		}
		else
		{
			if (_eraseDefault)
			{
				std::cout << "erasing default" << std::endl;
				writeShaderDefaultValueFile(datapath);
			}
			else
			{
				std::cout << "reading" << std::endl;
				//std::stringstream stream;
				//stream << file.rdbuf();
				//std::string fileAsString = stream.str();

				nlohmann::json variables;
				file >> variables;
				
				file.close();


				std::ofstream outfile(Rain::ResourcesRoot() + "sdlfkj.json");
				outfile << variables.dump(4);
				outfile.close();

				//readShaderDefaultValueFile(fileAsString);
			}
		}
		
	}


	void Material::writeShaderDefaultValueFile(const std::string& _datafilePath)
	{
		std::ofstream outfile(_datafilePath);

		nlohmann::json variables;
		for (size_t i = 0; i < m_shaderVariables.size(); ++i)
		{
			nlohmann::json json;
			json["name"] = m_shaderVariables[i].name;
			json["variable_type"] = Shader::GLSLVariableTypeToString(m_shaderVariables[i].variableType);
			json["glsl_type"] = GLUtils::GLTypeToString(m_shaderVariables[i].glslType);
			json["size"] = m_shaderVariables[i].size;
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

		outfile << std::setw(4) << variables;
		outfile.close();
	}

	void Material::readShaderDefaultValueFile(const std::string& _data)
	{
		nlohmann::json variables(_data);
		std::ofstream outfile(Rain::ResourcesRoot() + "sdlfkj.json");
		outfile << variables.dump(4);
		outfile.close();
		//for (size_t i = 0; i < m_shaderVariables.size(); ++i)
		//{
		//	std::string typestr = variables[m_shaderVariables[i].name]["glsl_type"].get<std::string>();
		//	GLenum value = GLUtils::StringToGLType(typestr);
		//	switch (value)
		//	{
		//	case GL_FLOAT:
		//		m_shaderVariables[i].value.m_float = variables[m_shaderVariables[i].name]["value"].get<float>();
		//		break;
		//	case GL_FLOAT_VEC2:
		//		m_shaderVariables[i].value.m_vec2 = variables[m_shaderVariables[i].name]["value"].get<glm::vec2>();
		//		break;
		//	case GL_FLOAT_VEC3:
		//		m_shaderVariables[i].value.m_vec3 = variables[m_shaderVariables[i].name]["value"].get<glm::vec3>();
		//		break;
		//	case GL_FLOAT_MAT4:
		//		m_shaderVariables[i].value.m_mat4 = variables[m_shaderVariables[i].name]["value"].get<glm::mat4>();
		//		break;
		//	case GL_SAMPLER_2D:
		//		m_shaderVariables[i].value.m_int = variables[m_shaderVariables[i].name]["value"].get<int>();
		//		break;
		//	case GL_SAMPLER_CUBE:
		//		m_shaderVariables[i].value.m_int = variables[m_shaderVariables[i].name]["value"].get<int>();
		//		break;
		//	default:
		//		m_shaderVariables[i].value.m_int = variables[m_shaderVariables[i].name]["value"].get<int>();
		//		break;
		//	}
		//}
	}
}


//    std::ifstream file;
//    std::string fileAsString;
//    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//    try
//    {
//        file.open(datapath);
//        std::stringstream stream;
//        stream << file.rdbuf();
//        fileAsString = stream.str();
//        file.close();
//    }
//    catch (std::ifstream::failure e)
//    {
//        if (!_eraseDefault)
//        {
//            std::cout << "ERROR::JSON::FILE_NOT_SUCCESFULLY_READ" << std::endl;
//            return -1;
//        }

//        std::cout << "Creating the file and pushing default values in it !" << std::endl;

//        nlohmann::json variables;
//        std::ofstream outfile(_shaderPath + ".json");
//        for (size_t i = 0; i < m_shaderVariables.size(); ++i)
//        {
//            nlohmann::json json;
//            json["name"] = m_shaderVariables[i].name;
//            json["variable_type"] = Shader::GLSLVariableTypeToString(m_shaderVariables[i].variableType);
//            json["glsl_type"] = GLUtils::GLTypeToString(m_shaderVariables[i].glslType);
//            json["size"] = m_shaderVariables[i].size;
//switch (m_shaderVariables[i].glslType)
//{
//case GL_FLOAT:
//	json["value"] = 0;
//	break;
//case GL_FLOAT_VEC2:
//	json["value"] = glm::vec2(0);
//	break;
//case GL_FLOAT_VEC3:
//	json["value"] = glm::vec3(0);
//	break;
//case GL_FLOAT_MAT4:
//	json["value"] = glm::mat4(0);
//	break;
//case GL_SAMPLER_2D:
//	json["value"] = 0;
//	break;
//case GL_SAMPLER_CUBE:
//	json["value"] = 0;
//	break;
//default:
//	json["value"] = 0;
//	break;
//}
//
//variables[m_shaderVariables[i].name] = json;
//        }

//        outfile << std::setw(4) << variables;
//        outfile.close();
//        return 0;
//    }


//    return 0;