#include "material.h"

#include "shader.h"
#include "utility/incl_3d.h"



namespace rain
{
    Material::Material()
    {
        std::string vertexpath = "";
        std::string fragmentpath = "";

        m_shader = new Shader();
        m_shader->init(vertexpath, fragmentpath);
        m_shader->use();



        
    }
}