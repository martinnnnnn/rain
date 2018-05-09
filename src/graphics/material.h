#pragma once




namespace rain
{
    class Shader;

    class Material
    {
    public:
        Material();

    private:
        Shader * m_shader;


    };
}