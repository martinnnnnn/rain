#pragma once


#include <string>

namespace rain
{
    class Shader;

    class Material
    {
    public:
        Material(const std::string& _shaderPath);

    private:
        Shader * m_shader;
        std::string m_shaderPath;

    };
}