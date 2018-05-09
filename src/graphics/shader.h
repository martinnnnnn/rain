#pragma once

#include "utility/incl_3d.h"

#include <string>
#include <vector>

namespace rain
{
    struct GLSLAttrib
    {
        GLSLAttrib(GLenum _type, char* _name, GLint _size) : type(_type), name(_name), size(_size) {}
        GLenum type;
        std::string name;
        GLint size;
    };

    struct GLSLUniform
    {
        GLSLUniform(GLenum _type, char* _name, GLint _size) : type(_type), name(_name), size(_size) {}
        GLenum type;
        std::string name;
        GLint size;
    };


    class Shader
    {
    public:
        GLuint id;

        Shader();
        Shader(const char* _vertexPath, const char* _fragmentPath);
        void init(const std::string& _vertexPath, const std::string& _fragmentPath);

        void load(const char* _vShaderCode, const char* _fShaderCode);
        void reload();
        std::string readFile(const char* _path);
    
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

        std::vector<GLSLAttrib> GetGLSLAttributes();
        std::vector<GLSLUniform> GetGLSLUniforms();

    private:
        void checkCompileErrors(unsigned int _shader, std::string _type);

        std::string m_vertexPath;
        std::string m_fragmentPath;
        GLuint m_vertexShader, m_fragmentShader;

    };
}
