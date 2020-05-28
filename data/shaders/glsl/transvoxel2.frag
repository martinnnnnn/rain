#version 420 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
  
uniform vec3 objectColor;

void main()
{
    FragColor = vec4(Normal, 1.0);
} 

