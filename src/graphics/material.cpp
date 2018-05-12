#include "material.h"


#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "utility/gl_utils.h"
#include "rain.h"
#include "shader.h"
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

    int Material::Init(const std::string& _shaderPath, bool _createDefaultIfMissing)
    {
        m_shaderPath = _shaderPath;
        std::string vertexpath = m_shaderPath + ".vs";
        std::string fragmentpath = m_shaderPath + ".fs";
        std::string defaultpath = m_shaderPath + ".json";

        m_shader = new Shader();
        m_shader->init(vertexpath, fragmentpath);
        m_shader->use();

        m_shaderVariables = m_shader->GetGLSLVariables();

        std::ifstream file;
        std::string fileAsString;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            file.open(defaultpath);
            std::stringstream stream;
            stream << file.rdbuf();
            fileAsString = stream.str();
            file.close();
        }
        catch (std::ifstream::failure e)
        {
            if (!_createDefaultIfMissing)
            {
                std::cout << "ERROR::JSON::FILE_NOT_SUCCESFULLY_READ" << std::endl;
                return -1;
            }

            std::cout << "Creating the file and pushing default variables in it !" << std::endl;

            nlohmann::json variables;
            std::ofstream outfile(_shaderPath + ".json");
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
            return 0;
        }

		/*nlohmann::json reader;
		reader.parse(fileAsString);

		for (int i = 0; i < m_shaderVariables.size(); ++i)
		{
			if (m_shaderVariables[i].variableType != GLSLVariableType::UNIFORM)
				continue;

			std::vector<float> value;
			switch (m_shaderVariables[i].glslType)
			{
			case GL_FLOAT:
				m_shader->setParameter(m_shaderVariables[i].name, reader[m_shaderVariables[i].name]["value"]);
				break;
			case GL_FLOAT_VEC2:
				for (nlohmann::json::iterator it = reader[m_shaderVariables[i].name]["value"].begin(); it != reader[m_shaderVariables[i].name]["value"].end(); ++it)
				{
					value.push_back(*it);
				}
				m_shader->setParameter(m_shaderVariables[i].name, value[0], value[1]);
				break;
			case GL_FLOAT_VEC3:
				for (nlohmann::json::iterator it = reader[m_shaderVariables[i].name]["value"].begin(); it != reader[m_shaderVariables[i].name]["value"].end(); ++it)
				{
					value.push_back(*it);
				}
				m_shader->setParameter(m_shaderVariables[i].name, value[0], value[1], value[2]);
				break;
			case GL_FLOAT_MAT4:
				for (nlohmann::json::iterator it = reader[m_shaderVariables[i].name]["value"].begin(); it != reader[m_shaderVariables[i].name]["value"].end(); ++it)
				{
					value.push_back(*it);
				}
				glm::mat4 mat;
				for (int i = 0; i < value.size(); ++i)
				{
					mat[i/4][i%4] = value[i];

				}
				m_shader->setParameter(m_shaderVariables[i].name, mat);
				break;
			case GL_SAMPLER_2D:
				m_shader->setParameter(m_shaderVariables[i].name, reader[m_shaderVariables[i].name]["value"]);
				break;
			case GL_SAMPLER_CUBE:
				break;
			default:
				break;
			}
		}*/


        return 0;
        //nlohmann::json json;
        //for (size_t i = 0; i < m_shaderVariables.size(); ++i)
        //{
        //    json[m_shaderVariables[i].name]["name"] = m_shaderVariables[i].name;
        //    json[m_shaderVariables[i].name]["variable_type"] = m_shaderVariables[i].variableType;
        //    json[m_shaderVariables[i].name]["glsl_type"] = m_shaderVariables[i].glslType;
        //    json[m_shaderVariables[i].name]["size"] = m_shaderVariables[i].size;
        //}
        //


        // read json
        // push default variables from json to shader
        // push model matrix (and other game object specific variables) from go transfom at link with obj.
    }
}