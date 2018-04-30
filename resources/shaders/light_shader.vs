#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 Position;
out vec3 Normal;
out vec2 TextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}