#include "material.h"

#include "shader.h"
#include "utility/incl_3d.h"



namespace rain
{
    Material::Material(const std::string& _shaderPath) :
        m_shaderPath(_shaderPath)
    {
        std::string vertexpath = _shaderPath + ".vs";
        std::string fragmentpath = _shaderPath + ".fs";
        std::string defaultpath = _shaderPath + ".json";

        m_shader = new Shader();
        m_shader->init(vertexpath, fragmentpath);
        m_shader->use();
        // read json
        // push default variables from json to shader
        // push model matrix (and other game object specific variables) from go transfom at link with obj.
    }
}