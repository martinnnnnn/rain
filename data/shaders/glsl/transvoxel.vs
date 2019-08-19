#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out VS_OUT
{
    vec3 Position;
    vec3 Normal;
} vs_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    vs_out.Position = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNorm;
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}