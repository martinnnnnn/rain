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

	


	void Shader::set(const std::string &_name, bool _value) const
	{
		GLint t = glGetUniformLocation(id, _name.c_str());
		glUniform1i(t, (int)_value);
	}
	void Shader::set(const std::string &_name, int _value) const
	{
		glUniform1i(glGetUniformLocation(id, _name.c_str()), _value);
	}

	void Shader::set(const std::string &_name, float _value) const
	{
		glUniform1f(glGetUniformLocation(id, _name.c_str()), _value);
	}

	void Shader::set(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
	}

	void Shader::set(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
	}

	void Shader::set(const std::string &name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
	}

	void Shader::set(const std::string &name, const glm::fvec2 &vector) const
	{
		glUniform2f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y);
	}

	void Shader::set(const std::string &name, const glm::fvec3 &vector) const
	{
		glUniform3f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y, vector.z);
	}

	void Shader::set(const std::string &name, const glm::fmat4 &matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}


	void Shader::check_compile_errors(unsigned int _shader, std::string _type)
	{
		int success;
		char infoLog[1024];
		char buffer[2048];

		if (_type != "PROGRAM")
		{
			glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(_shader, 1024, NULL, infoLog);
				char buffer[512];
				sprintf_s(buffer, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n-------------------------------------------------------\n", _type, infoLog);
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
				sprintf_s(buffer, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n-------------------------------------------------------\n", _type, infoLog);
				OutputDebugStringA(buffer);
			}
		}
	}
}