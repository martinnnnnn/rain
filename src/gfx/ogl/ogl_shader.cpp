#include "ogl_shader.h"


#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>

namespace rain
{
	
	Shader::Shader()
	{
	}

	void Shader::reload()
	{
		//std::string s1 = readFile(m_vertexPath.c_str());
		//std::string s2 = readFile(m_fragmentPath.c_str());
		//const char* vShaderCode = s1.c_str();
		//const char* fShaderCode = s2.c_str();

		//load(vShaderCode, fShaderCode);
	}

	bool Shader::load(const std::string& _vertex_file, const std::string& _fragment_file, const std::string& _geometry_file)
	{
		bool retval = true;
		retval = retval && m_vertex_file.open(_vertex_file);
		retval = retval && m_fragment_file.open(_fragment_file);
		if (_geometry_file != "")
		{
			retval = retval && m_geometry_file.open(_geometry_file);
		}

		if (retval)
		{
			const char* vertex = m_vertex_file.read().c_str();
			const char* fragment = m_fragment_file.read().c_str();
			const char* geometry = m_geometry_file.read().c_str();

			load(m_vertex_file.read().c_str(), m_fragment_file.read().c_str(), m_geometry_file.read().c_str());
		}
		m_vertex_file.close();
		m_fragment_file.close();
		m_geometry_file.close();

		return true;
	}


	void Shader::load(const char* _vertex_code, const char* _fragment_code, const char* _geometry_code)
	{
		GLuint m_vertexShader;
		GLuint m_fragmentShader;
		GLuint m_geometryShader;

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
		glDeleteShader(m_geometryShader);
	}

	void Shader::use()
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
				char buffer[512];
				sprintf_s(buffer, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n-------------------------------------------------------\n", _type.c_str(), infoLog);
				OutputDebugStringA(buffer);

			}
		}
		else
		{
			glGetProgramiv(_shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(_shader, 1024, NULL, infoLog);
				char buffer[512];
				sprintf_s(buffer, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n-------------------------------------------------------\n", _type.c_str(), infoLog);
				OutputDebugStringA(buffer);
			}
		}
	}
}