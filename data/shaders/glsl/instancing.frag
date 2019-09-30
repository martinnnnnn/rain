// #version 330 core


// out vec4 FragColor;

// in VS_OUT
// {
//     vec3 Position;
//     vec3 Normal;
//     vec2 TextCoord;
// } fs_in;

// uniform vec3 lightDirection;
// uniform vec3 lightDiff;

// void main()
// {
//     vec3 lightDir = normalize(-lightDirection);
//     vec3 diffuseLight = max(dot(fs_in.Normal, lightDir), 0.0) * lightDiff;
//     FragColor =  vec4(1.0f, 0.5f, 0.2f, 1.0f);
//     // FragColor =  vec4(1.0f, 0.5f, 0.2f, 1.0f) * vec4(diffuseLight, 1.0);
// }

#version 330 core
out vec4 FragColor;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT
{
    vec3 Position;
    vec3 Normal;
    vec2 TextCoord;
} fs_in;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform sampler2D text1;

vec3 compute_direction_light(DirLight light, vec3 normal, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.Position);

    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 ambient = dirLight.ambient * vec3(texture(text1, fs_in.TextCoord));

    FragColor = vec4(ambient, 1.0);
}

// vec3 compute_direction_light(DirLight light, vec3 normal, vec3 viewDir)
// {
//     vec3 lightDir = normalize(-light.direction);
//     float diff = max(dot(normal, lightDir), 0.0);

//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//     return (ambient + diffuse + specular);
// }