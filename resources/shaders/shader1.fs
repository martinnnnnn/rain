#version 330 core
out vec4 FragColor;

in vec2 TextCoord;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform float mixValue;

void main()
{
    FragColor = mix(texture(uTexture0, TextCoord), texture(uTexture1, TextCoord), mixValue);
} 

