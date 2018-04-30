#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 TextCoord;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform float mixValue;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
void main()
{
    // ambient
    float ambientStrenght = 0.2;
    vec3 ambientLight = normalize(lightColor) * ambientStrenght;

    // diffuse
    vec3 lightDir = normalize(lightPos - Position);
    vec3 diffuseLight = max(dot(Normal, lightDir), 0.0) * lightColor;

    // specular
    float specularStrenght = 0.5;
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, Normal);
    vec3 specular = pow(max(dot(viewDir,reflectDir),0.0),32) * lightColor * specularStrenght;

    FragColor = mix(texture(uTexture0, TextCoord), texture(uTexture1, TextCoord), mixValue);
    FragColor = FragColor * vec4(ambientLight + diffuseLight + specular, 1);
} 

