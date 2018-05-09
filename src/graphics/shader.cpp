#include "shader.h"


#include <fstream>
#include <sstream>
#include <iostream>

#include "utility/incl_3d.h"



namespace rain
{

    Shader::Shader()
    {

    }

    void Shader::init(const std::string& _vertexPath, const std::string& _fragmentPath)
    {
        m_vertexPath = _vertexPath;
        m_fragmentPath = _fragmentPath;

        std::string s1 = readFile(m_vertexPath.c_str());
        std::string s2 = readFile(m_fragmentPath.c_str());
        const char* vShaderCode = s1.c_str();
        const char* fShaderCode = s2.c_str();

        id = glCreateProgram();
        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        load(vShaderCode, fShaderCode);
    }

    Shader::Shader(const char* _vertexPath, const char* _fragmentPath)
    {
        init(_vertexPath, _fragmentPath);
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
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        return fileContent;
    }

    void Shader::reload()
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

    std::vector<GLSLAttrib> Shader::GetGLSLAttributes()
    {
        std::vector<GLSLAttrib> attribs;

        GLint count;
        GLint size;
        GLenum type;
        const GLsizei bufSize = 16;
        GLchar name[16];
        GLsizei length;

        glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &count);

        for (GLint i = 0; i < count; ++i)
        {
            glGetActiveAttrib(id, i, bufSize, &length, &size, &type, name);
            attribs.push_back(GLSLAttrib(type, name, size));
        }
        return attribs;
    }

    std::vector<GLSLUniform> Shader::GetGLSLUniforms()
    {
        std::vector<GLSLUniform> uniforms;

        GLint count;
        GLint size;
        GLenum type;
        const GLsizei bufSize = 16;
        GLchar name[16];
        GLsizei length;

        glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

        for (GLint i = 0; i < count; ++i)
        {
            glGetActiveUniform(id, i, bufSize, &length, &size, &type, name);
            uniforms.push_back(GLSLUniform(type, name, size));
        }
        return uniforms;
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