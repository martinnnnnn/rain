#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTextureCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Position = vec3(model * vec4(aPos, 1.0));
    TextCoord = aTextureCoord;
    Normal = normalize(mat3(transpose(inverse(model))) * aNorm);  
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}