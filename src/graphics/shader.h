#pragma once

#include "utility/stdincl_3d.h"

#include <string>

namespace rain
{
    class Shader
    {
    public:
        GLuint id;

        Shader();
        Shader(const char* _vertexPath, const char* _fragmentPath);
        void init(const std::string& _vertexPath, const std::string& _fragmentPath);
        //void init(const char* vertexPath, const char* fragmentPath);

        void load(const char* _vShaderCode, const char* _fShaderCode);
        void reload();
        std::string readFile(const char* _path);
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
        void checkCompileErrors(unsigned int _shader, std::string _type);

        std::string m_vertexPath;
        std::string m_fragmentPath;
        GLuint m_vertexShader, m_fragmentShader;

    };
}
