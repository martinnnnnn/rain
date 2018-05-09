#pragma once


#include <string>
#include <vector>

#include "shader.h"

namespace rain
{
    
    class Material
    {
    public:
        Material() {}
        Material(const std::string& _shaderPath, bool _createDefaultIfMissing = true);

        int Init(const std::string& _shaderPath, bool _createDefaultIfMissing = true);
    private:
        Shader * m_shader;
        std::string m_shaderPath;
        std::vector<GLSLVariable> m_shaderVariables;
    };
}