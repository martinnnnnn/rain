#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>


class Shader
{
public:
    GLuint _ID;

    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);
    void init(const char* vertexPath, const char* fragmentPath);

    void load(const char* vShaderCode, const char* fShaderCode);
    void reload();
    std::string readFile(const char* path);
    //void reload(const char* vertexPath, const char* fragmentPath);
    
    
    void use();
    void setParameter(const std::string &name, bool value) const;
    void setParameter(const std::string &name, int value) const;
    void setParameter(const std::string &name, float x) const;
    void setParameter(const std::string &name, float x, float y) const;
    void setParameter(const std::string &name, float x, float y, float z) const;
    void setParameter(const std::string &name, float x, float y, float z, float w) const;
    void setParameter(const std::string &name, const glm::fvec2 &vector) const;
    void setParameter(const std::string &name, const glm::fvec3 &vector) const;
    void setParameter(const std::string &name, const glm::fmat4 &matrix);
    //void setParameter(const std::string &name, CurrentTextureType);

private:
    void checkCompileErrors(unsigned int shader, std::string type);

    const char* _vertexPath;
    const char* _fragmentPath;
    GLuint _vertexShader, _fragmentShader;

};
#endif