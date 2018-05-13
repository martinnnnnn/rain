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


		// holds shader, textures

		// use
		//	bind texture
		//	use shader
		// set parameters ?



    private:
        Shader * m_shader;
        std::string m_shaderPath;
        std::vector<GLSLVariable> m_shaderVariables;
    };
}