#include "ogl_shader.h"


#include <fstream>
#include <sstream>
#include <iostream>

#include "gtc/type_ptr.hpp"

#include "engine/core/context.h"
#include "engine/win32/win32_helpers.h"

#include "ogl_renderer.h"

namespace rain::engine
{
	
	Shader::Shader()
        : vertex_object(0)
        , fragment_object(0)
        , geometry_object(0)
	{
	}

	bool Shader::load(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path)
	{
        bool success = false;

        std::string vertex_code;
        std::string fragment_code;
        std::string geometry_code;

        if (!vertex_path.empty() && !core::File::exists(vertex_path))
        {
            RAIN_LOG_WARNING("[SHADER::open] Failed to open vertex shader with path : \n%s", vertex_path.c_str());
        }
        else
        {
			core::File::read(vertex_path, vertex_code);
        }

        if (!fragment_path.empty() && !core::File::exists(fragment_path))
        {
            RAIN_LOG_WARNING("[SHADER::open] Failed to open fragment shader with path : \n%s", fragment_path.c_str());
        }
        else
        {
			core::File::read(fragment_path, fragment_code);
        }

        if (!geometry_path.empty() && !core::File::exists(geometry_path))
        {
            RAIN_LOG_WARNING("[SHADER::open] Failed to open geometry shader with path : \n%s", geometry_path.c_str());
        }
        else
        {
			core::File::read(geometry_path, geometry_code);
        }

        if (vertex_code != "" && fragment_code != "")
        {
            load(vertex_code.c_str(), fragment_code.c_str(), geometry_code != "" ? geometry_code.c_str() : nullptr);
            success = true;
        }

		return success;
	}

    void Shader::gpu_load(GLuint obj, const char* code)
    {
#ifndef SPIRV
        glShaderSource(obj, 1, &code, NULL);
        glCompileShader(obj);
        check_compile_errors(obj, "VERTEX");
#else
        glShaderBinary(1, &obj, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, code, strlen(code));
        gl_specialize_shader(obj, "main", 0, 0, 0);
#endif

    }

	void Shader::load(const char* _vertex_code, const char* _fragment_code, const char* _geometry_code)
	{
		id = glCreateProgram();

		vertex_object = glCreateShader(GL_VERTEX_SHADER);
        gpu_load(vertex_object, _vertex_code);
		//glShaderSource(vertex_object, 1, &_vertex_code, NULL);
		//glCompileShader(vertex_object);
		//check_compile_errors(vertex_object, "VERTEX");

		fragment_object = glCreateShader(GL_FRAGMENT_SHADER);
        gpu_load(fragment_object, _fragment_code);
  //      glShaderSource(fragment_object, 1, &_fragment_code, NULL);
		//glCompileShader(fragment_object);	
		//check_compile_errors(fragment_object, "FRAGMENT");

		if (_geometry_code != nullptr)
		{
			geometry_object = glCreateShader(GL_GEOMETRY_SHADER);
            gpu_load(geometry_object, _geometry_code);
   //         glShaderSource(geometry_object, 1, &_geometry_code, NULL);
			//glCompileShader(geometry_object);
			//check_compile_errors(geometry_object, "GEOMETRY");
		}

		glAttachShader(id, vertex_object);
		glAttachShader(id, fragment_object);
		if (_geometry_code != nullptr)
		{
			glAttachShader(id, geometry_object);
		}

		glLinkProgram(id);
		check_compile_errors(id, "PROGRAM");


		glDeleteShader(vertex_object);
		glDeleteShader(fragment_object);
        if (_geometry_code != nullptr)
        {
		    glDeleteShader(geometry_object);
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

    u32 Shader::set(const std::string &_name, float x, float y, float z, float w) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniform4f(parameter_id, x, y, z, w);
        return parameter_id;
    }

    u32 Shader::set(const std::string &_name, const glm::vec2 &_vector) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniform2f(parameter_id, _vector.x, _vector.y);
        return parameter_id;
    }

    u32 Shader::set(const std::string &_name, const glm::vec3 &_vector) const
	{
        u32 parameter_id = (u32)glGetUniformLocation(id, _name.c_str());
        glUniform3f(parameter_id, _vector.x, _vector.y, _vector.z);
        return parameter_id;
    }

    u32 Shader::set(const std::string& name, const glm::vec4& v) const
    {
        u32 parameter_id = (u32)glGetUniformLocation(id, name.c_str());
        glUniform4f(parameter_id, v.x, v.y, v.z, v.w);
        return parameter_id;
    }

    u32 Shader::set(const std::string &_name, const glm::mat4 &_matrix) const
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
    void Shader::set(u32 _parameterID, const glm::vec2& _vector) const
    {
        glUniform2f(_parameterID, _vector.x, _vector.y);
    }
    void Shader::set(u32 _parameterID, const glm::vec3& _vector) const
    {
        glUniform3f(_parameterID, _vector.x, _vector.y, _vector.z);
    }
    void Shader::set(u32 _parameterID, const glm::vec4& v) const
    {
        glUniform4f(_parameterID, v.x, v.y, v.z, v.w);
    }
    void Shader::set(u32 _parameterID, const glm::mat4& _matrix) const
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