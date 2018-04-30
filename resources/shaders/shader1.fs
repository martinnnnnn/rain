#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 TextCoord;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform float mixValue;

uniform vec3 viewPos;

uniform Material mat;
uniform Light light;

void main()
{
    // ambient
    // float ambientStrenght = 0.1;
    vec3 ambientLight = normalize(light.ambient/* * ambientStrenght*/ * mat.ambient);

    // diffuse
    vec3 lightDir = normalize(light.position - Position);
    vec3 diffuseLight = max(dot(Normal, lightDir), 0.0) * light.diffuse * mat.diffuse;

    // specular
    // float specularStrenght = 0.5;
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, Normal);
    vec3 specular = pow(max(dot(viewDir,reflectDir),0.0),mat.shininess) * light.specular /** specularStrenght*/ * mat.specular;

    FragColor = mix(texture(uTexture0, TextCoord), texture(uTexture1, TextCoord), mixValue);
    FragColor = FragColor * vec4(ambientLight + diffuseLight + specular, 1);
} 

