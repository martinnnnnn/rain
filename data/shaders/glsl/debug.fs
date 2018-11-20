#version 330 core

in vec3 VS_OUT_color;

out vec4 FragColor;

void main()
{
    FragColor =  vec4(VS_OUT_color, 1.0);
}