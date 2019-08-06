#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTextureCoord;
layout (location = 3) in mat4 aInstanceMatrix;

out VS_OUT
{
    vec3 Position;
    vec3 Normal;
    vec2 TextCoord;
} vs_out;

uniform mat4 view;
uniform mat4 proj;

void main()
{
    vs_out.Position = vec3(aInstanceMatrix * vec4(aPos, 1.0));
    vs_out.TextCoord = aTextureCoord;
    vs_out.Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNorm;
    gl_Position = proj * view * aInstanceMatrix * vec4(aPos, 1.0);
}