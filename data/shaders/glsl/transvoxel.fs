#version 330 core

out vec4 FragColor;

in VS_OUT
{
    vec3 Position;
    vec3 Normal;
} fs_in;

uniform vec3 lightDirection;
uniform vec3 lightDiff;

void main()
{
    vec3 lightDir = normalize(-lightDirection);
    vec3 diffuseLight = max(dot(fs_in.Normal, lightDir), 0.0) * lightDiff;
    FragColor =  vec4(1.0f, 0.5f, 0.2f, 1.0f) * vec4(diffuseLight, 1.0);
}
