#include "shader.h"


#include <fstream>
#include <sstream>
#include <iostream>

#include "utility/incl_3d.h"
#include "utility/string_utils.h"


namespace rain
{
    namespace GLSL
    {
        std::string TypeToString(Type value)
        {
            switch (value)
            {
            case Type::ATTRIBUTE:
                return "attribute";
            case Type::UNIFORM:
                return "uniform";
            default:
                return "unknown";
            }
        }
    }

    Shader::Shader()
    {

    }

    void Shader::init(const std::string& _vertexPath, const std::string& _fragmentPath)
    {
        m_vertexPath = _vertexPath;
        m_fragmentPath = _fragmentPath;

        id = glCreateProgram();
        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		std::string s1 = readFile(m_vertexPath.c_str());
		std::string s2 = readFile(m_fragmentPath.c_str());

        const char* vShaderCode = s1.c_str();
        const char* fShaderCode = s2.c_str();
        load(vShaderCode, fShaderCode);
    }

    std::string Shader::readFile(const char* _path)
    {
        std::string fileContent("");
        std::ifstream file;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            file.open(_path);
            std::stringstream stream;
            stream << file.rdbuf();
            fileContent = stream.str();

            file.close();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ : " << _path << std::endl;
        }

        return fileContent;
    }

    void Shader::Reload()
    {
        std::string s1 = readFile(m_vertexPath.c_str());
        std::string s2 = readFile(m_fragmentPath.c_str());
        const char* vShaderCode = s1.c_str();
        const char* fShaderCode = s2.c_str();

        load(vShaderCode, fShaderCode);
    }

    void Shader::load(const char* _vShaderCode, const char* _fShaderCode)
    {
        glShaderSource(m_vertexShader, 1, &_vShaderCode, NULL);
        glCompileShader(m_vertexShader);
        checkCompileErrors(m_vertexShader, "VERTEX of " + m_vertexPath);

        glShaderSource(m_fragmentShader, 1, &_fShaderCode, NULL);
        glCompileShader(m_fragmentShader);
        checkCompileErrors(m_fragmentShader, "FRAGMENT of " + m_fragmentPath);

        glAttachShader(id, m_vertexShader);
        glAttachShader(id, m_fragmentShader);
        glLinkProgram(id);
        checkCompileErrors(id, "PROGRAM");

        glDeleteShader(m_vertexShader);
        glDeleteShader(m_fragmentShader);
    }

    void Shader::use()
    {
        glUseProgram(id);
    }

    std::vector<GLSL::Variable> Shader::GetGLSLVariablesSimple() const
    {
        std::vector<GLSL::Variable> variables;

        GLint count;
        GLint size;
        GLenum glslType;
        const GLsizei bufSize = 32;
        GLchar name[bufSize];
        GLsizei length;

        // retrieving attributes
        glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &count);
        for (GLint i = 0; i < count; ++i)
        {
            glGetActiveAttrib(id, i, bufSize, &length, &size, &glslType, name);

            variables.push_back(GLSL::Variable(GLSL::Type::ATTRIBUTE, glslType, name, size));
        }

        // retrieving uniforms
        glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);
        for (GLint i = 0; i < count; ++i)
        {
            glGetActiveUniform(id, i, bufSize, &length, &size, &glslType, name);

            variables.push_back(GLSL::Variable(GLSL::Type::UNIFORM, glslType, name, size));
        }

        return variables;
    }

    std::unordered_map<std::string, std::vector<GLSL::Variable>> Shader::GetGLSLVariablesFancy() const
    {
        std::unordered_map<std::string, std::vector<GLSL::Variable>> variables;

        GLint count;
        GLint size;
        GLenum glslType;
        const GLsizei bufSize = 32;
        GLchar name[bufSize];
        GLsizei length;
        
        // retrieving attributes
        glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &count);
        for (GLint i = 0; i < count; ++i)
        {
            glGetActiveAttrib(id, i, bufSize, &length, &size, &glslType, name);

            // spliting the name to check if it's part of a struct
            std::vector<std::string> elements = String::split(name, '.');

            std::string splitedName = elements[0];
            std::string key = "";
            if (elements.size() > 1)
            {
                splitedName = elements[1];
                key = elements[0];
            }
            variables[key].push_back(GLSL::Variable(GLSL::Type::ATTRIBUTE, glslType, splitedName, size));
        }

        // retrieving uniforms
        glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);
        for (GLint i = 0; i < count; ++i)
        {
            glGetActiveUniform(id, i, bufSize, &length, &size, &glslType, name);

            // spliting the name to check if it's part of a struct
            std::vector<std::string> elements = String::split(name, '.');

            std::string splitedName = elements[0];
            std::string key = "";
            if (elements.size() > 1)
            {
                splitedName = elements[1];
                key = elements[0];
            }
            variables[key].push_back(GLSL::Variable(GLSL::Type::UNIFORM, glslType, splitedName, size));
        }

        return variables;
    }


    void Shader::setParameter(const std::string &_name, bool _value) const
    {
        glUniform1i(glGetUniformLocation(id, _name.c_str()), (int)_value);
    }
    void Shader::setParameter(const std::string &_name, int _value) const
    {
        glUniform1i(glGetUniformLocation(id, _name.c_str()), _value);
    }

    void Shader::setParameter(const std::string &_name, float _value) const
    {
        glUniform1f(glGetUniformLocation(id, _name.c_str()), _value);
    }

    void Shader::setParameter(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }

    void Shader::setParameter(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }

    void Shader::setParameter(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }

    void Shader::setParameter(const std::string &name, const glm::fvec2 &vector) const
    {
        glUniform2f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y);
    }

    void Shader::setParameter(const std::string &name, const glm::fvec3 &vector) const
    {
        glUniform3f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y, vector.z);
    }

    void Shader::setParameter(const std::string &name, const glm::fmat4 &matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }


    void Shader::checkCompileErrors(unsigned int _shader, std::string _type)
    {
        int success;
        char infoLog[1024];
        if (_type != "PROGRAM")
        {
            glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(_shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << _type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(_shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(_shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << _type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
}