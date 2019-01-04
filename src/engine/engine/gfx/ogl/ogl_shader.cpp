#include "ogl_shader.h"


#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/core/context.h"
#include "engine/core/logger.h"
#include "engine/win32/win32_helpers.h"

namespace rain::engine
{
	
	Shader::Shader()
	{
	}

	bool Shader::load(const std::string& _vertex_file, const std::string& _fragment_file, const std::string& _geometry_file)
	{
        bool success = false;

        std::string vertex_code = "";
        std::string fragment_code = "";
        std::string geometry_code = "";

        if (m_vertex_file.open(_vertex_file))
        {
            vertex_code = m_vertex_file.read();
        }
        if (m_fragment_file.open(_fragment_file))
        {
            fragment_code = m_fragment_file.read();
        }
        if (m_geometry_file.open(_geometry_file))
        {
            geometry_code = m_geometry_file.read();
        }

        if (vertex_code != "" && fragment_code != "")
        {
            load(vertex_code.c_str(), fragment_code.c_str(), geometry_code != "" ? geometry_code.c_str() : nullptr);
            success = true;
        }

		m_vertex_file.close();
		m_fragment_file.close();
		m_geometry_file.close();

		return success;
	}


	void Shader::load(const char* _vertex_code, const char* _fragment_code, const char* _geometry_code)
	{
		id = glCreateProgram();
		m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(m_vertexShader, 1, &_vertex_code, NULL);
		glCompileShader(m_vertexShader);
		check_compile_errors(m_vertexShader, "VERTEX");

		glShaderSource(m_fragmentShader, 1, &_fragment_code, NULL);
		glCompileShader(m_fragmentShader);	
		check_compile_errors(m_fragmentShader, "FRAGMENT");

		if (_geometry_code != nullptr)
		{
			m_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(m_geometryShader, 1, &_geometry_code, NULL);
			glCompileShader(m_geometryShader);
			check_compile_errors(m_geometryShader, "GEOMETRY");
		}

		glAttachShader(id, m_vertexShader);
		glAttachShader(id, m_fragmentShader);
		if (_geometry_code != nullptr)
		{
			glAttachShader(id, m_geometryShader);
		}

		glLinkProgram(id);
		check_compile_errors(id, "PROGRAM");


		glDeleteShader(m_vertexShader);
		glDeleteShader(m_fragmentShader);
        if (_geometry_code != nullptr)
        {
		    glDeleteShader(m_geometryShader);
        }
	}

    void Shader::unload()
    {
        glDeleteProgram(id);
    }

	void Shader::use() const
	{
		glUseProgram(id);
	}

    u32 Shader::set(const std::string &_name, bool _value) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
		glUniform1i(parameter_id, (int)_value);
        return parameter_id;
	}

    u32 Shader::set(const std::string &_name, int _value) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniform1i(parameter_id, _value);
        return parameter_id;
    }

    u32 Shader::set(const std::string &_name, float _value) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniform1f(parameter_id, _value);
        return parameter_id;
    }

    u32 Shader::set(const std::string &_name, float _x, float _y) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniform2f(parameter_id, _x, _y);
        return parameter_id;
    }

    u32 Shader::set(const std::string &_name, float _x, float _y, float _z) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniform3f(parameter_id, _x, _y, _z);
        return parameter_id;
    }

    u32 Shader::set(const std::string &_name, float _x, float _y, float _z, float _w) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniform4f(parameter_id, _x, _y, _z, _w);
        return parameter_id;
    }

    u32 Shader::set(const std::string &_name, const glm::fvec2 &_vector) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniform2f(parameter_id, _vector.x, _vector.y);
        return parameter_id;
    }

    u32 Shader::set(const std::string &_name, const glm::fvec3 &_vector) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniform3f(parameter_id, _vector.x, _vector.y, _vector.z);
        return parameter_id;
    }

    u32 Shader::set(const std::string &_name, const glm::fmat4 &_matrix) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniformMatrix4fv(parameter_id, 1, GL_FALSE, glm::value_ptr(_matrix));
        return parameter_id;
    }

    void Shader::set(u32 _parameterID, bool _value) const
    {
        glUniform1i(_parameterID, (int)_value);
    }
    void Shader::set(u32 _parameterID, int _value) const
    {
        glUniform1i(_parameterID, _value);
    }
    void Shader::set(u32 _parameterID, float _x) const
    {
        glUniform1f(_parameterID, _x);
    }
    void Shader::set(u32 _parameterID, float _x, float _y) const
    {
        glUniform2f(_parameterID, _x, _y);
    }
    void Shader::set(u32 _parameterID, float _x, float _y, float _z) const
    {
        glUniform3f(_parameterID, _x, _y, _z);
    }
    void Shader::set(u32 _parameterID, float _x, float _y, float _z, float _w) const
    {
        glUniform4f(_parameterID, _x, _y, _z, _w);
    }
    void Shader::set(u32 _parameterID, const glm::fvec2& _vector) const
    {
        glUniform2f(_parameterID, _vector.x, _vector.y);
    }
    void Shader::set(u32 _parameterID, const glm::fvec3& _vector) const
    {
        glUniform3f(_parameterID, _vector.x, _vector.y, _vector.z);

    }
    void Shader::set(u32 _parameterID, const glm::fmat4& _matrix) const
    {
        glUniformMatrix4fv(_parameterID, 1, GL_FALSE, glm::value_ptr(_matrix));
    }

	void Shader::check_compile_errors(unsigned int _shader, const std::string& _type)
	{
		int success;
		char infoLog[1024];

		if (_type != "PROGRAM")
		{
			glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(_shader, 1024, NULL, infoLog);
                RAIN_LOG("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n-------------------------------------------------------\n", _type.c_str(), infoLog);
			}
		}
		else
		{
			glGetProgramiv(_shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(_shader, 1024, NULL, infoLog);
                RAIN_LOG("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n-------------------------------------------------------\n", _type.c_str(), infoLog);
			}
		}
	}
}