#version 420 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 color;

out vec4 out_color;

uniform mat4 projection;

void main()
{   
    out_color = vec4(color, 1.0);
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
}  