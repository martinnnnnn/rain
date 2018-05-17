#pragma once


#include <string>
#include <vector>
#include <fstream>

#include "shader.h"
#include "core/component.h"

namespace rain
{
    class Texture;

    class Material : public Component
    {
    public:
        Material() {}
        Material(const std::string& _shaderPath, bool _eraseDefault = true);

        void Init(const std::string& _shaderPath, bool _eraseDefault = true);
        

		// holds shader, textures

		// use
		//	bind texture
		//	use shader
		// set parameters ?
	private:
		void writeShaderDefaultValueFile(const std::string& _datafilePath);
		void readShaderDefaultValueFile(std::ifstream& _inputFile);

    private:
        Shader * m_shader;
        std::string m_shaderPath;
        std::vector<GLSLVariable> m_shaderVariables;
    };
}