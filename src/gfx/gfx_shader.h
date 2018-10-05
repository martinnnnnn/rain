#pragma once

#include "utility/incl_3d.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace rain
{

    namespace GLSL
    {
        enum class Type
        {
            ATTRIBUTE,
            UNIFORM,
            LAST_INDEX
        };

        std::string TypeToString(Type value);


        struct Variable
        {
            //Variable(GLSL::Type _variableType, GLenum _glslType, char* _name, GLint _size) : variableType(_variableType), glslType(_glslType), name(_name), size(_size) {}
            //Variable(GLSL::Type _variableType, GLenum _glslType, const std::string& _name, GLint _size) : variableType(_variableType), glslType(_glslType), name(_name), size(_size) {}

            GLSL::Type variableType;
            GLenum glslType;
            std::string name;
            GLuint textureId;
            GLint size;
            union Value
            {
                int m_int;
                float m_float;
                glm::vec2 m_vec2;
                glm::vec3 m_vec3;
                glm::mat4 m_mat4;
            };
            Value value;
        };
    }




    class Shader
    {
    public:
        GLuint id;

        Shader();
        void init(const std::string& _vertexPath, const std::string& _fragmentPath);
        void use();
        void Reload();

        void setParameter(const std::string &name, bool value) const;
        void setParameter(const std::string &name, int value) const;
        void setParameter(const std::string &name, float x) const;
        void setParameter(const std::string &name, float x, float y) const;
        void setParameter(const std::string &name, float x, float y, float z) const;
        void setParameter(const std::string &name, float x, float y, float z, float w) const;
        void setParameter(const std::string &name, const glm::fvec2 &vector) const;
        void setParameter(const std::string &name, const glm::fvec3 &vector) const;
        void setParameter(const std::string &name, const glm::fmat4 &matrix);

        std::vector<GLSL::Variable> GetGLSLVariablesSimple() const;
        std::unordered_map<std::string, std::vector<GLSL::Variable>> GetGLSLVariablesFancy() const;

    private:
        void load(const char* _vShaderCode, const char* _fShaderCode);
        std::string readFile(const char* _path);

        void checkCompileErrors(unsigned int _shader, std::string _type);

        std::string m_vertexPath;
        std::string m_fragmentPath;
        GLuint m_vertexShader;
        GLuint m_fragmentShader;
    };
}