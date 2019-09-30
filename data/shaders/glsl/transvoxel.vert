// #version 420 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNorm;

// // out VS_OUT
// // {
// //     vec3 Position;
// //     vec3 Normal;
// // } vs_out;

// out vec3 Normal;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

// void main()
// {
//     Normal = mat3(transpose(inverse(model))) * aNorm;
//     // vs_out.Position = vec3(model * vec4(aPos, 1.0));
//     // vs_out.Normal = mat3(transpose(inverse(model))) * aNorm;
//     gl_Position = projection * view * model * vec4(aPos, 1.0);
// }

#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}