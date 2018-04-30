#include "shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <fstream>
#include <sstream>
#include <iostream>




Shader::Shader()
{

}

void Shader::init(const std::string& vertexPath, const std::string& fragmentPath)
{
    _vertexPath = vertexPath;
    _fragmentPath = fragmentPath;

    std::string s1 = readFile(_vertexPath.c_str());
    std::string s2 = readFile(_fragmentPath.c_str());
    const char* vShaderCode = s1.c_str();
    const char* fShaderCode = s2.c_str();

    _ID = glCreateProgram();
    _vertexShader = glCreateShader(GL_VERTEX_SHADER);
    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    load(vShaderCode, fShaderCode);
}

//void Shader::init(const char* vertexPath, const char* fragmentPath)
//{
//    _vertexPath = (char*)malloc(length_of_a);
//    memcpy(b, a, length_of_a);
//
//    _vertexPath = vertexPath;
//    _fragmentPath = fragmentPath;
//
//    std::string s1 = readFile(_vertexPath);
//    std::string s2 = readFile(_fragmentPath);
//    const char* vShaderCode = s1.c_str();
//    const char* fShaderCode = s2.c_str();
//
//    _ID = glCreateProgram();
//    _vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//
//    load(vShaderCode, fShaderCode);
//}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    init(vertexPath, fragmentPath);
}

std::string Shader::readFile(const char* path)
{
    std::string fileContent("");
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);
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
    std::string s1 = readFile(_vertexPath.c_str());
    std::string s2 = readFile(_fragmentPath.c_str());
    const char* vShaderCode = s1.c_str();
    const char* fShaderCode = s2.c_str();

    load(vShaderCode, fShaderCode);
}

void Shader::load(const char* vShaderCode, const char* fShaderCode)
{
    int success;
    char infoLog[512];

    glShaderSource(_vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(_vertexShader);
    checkCompileErrors(_vertexShader, "VERTEX of " + _vertexPath);

    glShaderSource(_fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(_fragmentShader);
    checkCompileErrors(_fragmentShader, "FRAGMENT of " + _fragmentPath);

    glAttachShader(_ID, _vertexShader);
    glAttachShader(_ID, _fragmentShader);
    glLinkProgram(_ID);
    checkCompileErrors(_ID, "PROGRAM");

    glDeleteShader(_vertexShader);
    glDeleteShader(_fragmentShader);
}

void Shader::use()
{
    glUseProgram(_ID);
}
void Shader::setParameter(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(_ID, name.c_str()), (int)value);
}
void Shader::setParameter(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(_ID, name.c_str()), value);
}

void Shader::setParameter(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(_ID, name.c_str()), value);
}

void Shader::setParameter(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(_ID, name.c_str()), x, y);
}

void Shader::setParameter(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(_ID, name.c_str()), x, y, z);
}

void Shader::setParameter(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(_ID, name.c_str()), x, y, z, w);
}

void Shader::setParameter(const std::string &name, const glm::fvec2 &vector) const
{
    glUniform2f(glGetUniformLocation(_ID, name.c_str()), vector.x, vector.y);
}

void Shader::setParameter(const std::string &name, const glm::fvec3 &vector) const
{
    glUniform3f(glGetUniformLocation(_ID, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::setParameter(const std::string &name, const glm::fmat4 &matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}