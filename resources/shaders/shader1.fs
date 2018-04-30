#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 TextCoord;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    float shininess;
};

struct LightDirectional
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


uniform vec3 viewPos;

uniform Material mat;
uniform LightDirectional dirLight;



vec3 GetDirLight(LightDirectional light, vec3 viewDir);


void main()
{
    //vec3 lightDir = normalize(light.position - Position);
    vec3 viewDir = normalize(viewPos - Position);
    //vec3 reflectDir = reflect(-lightDir, Normal);



    // ambient
    //vec3 ambientLight = light.ambient * vec3(texture(mat.diffuse, TextCoord));

    // diffuse
    //vec3 diffuseLight = max(dot(Normal, lightDir), 0.0) * light.diffuse * vec3(texture(mat.diffuse, TextCoord));

    // specular
    //vec3 specular = pow(max(dot(viewDir,reflectDir), 0.0), mat.shininess) * light.specular * vec3(texture(mat.specular, TextCoord));

    // emissive
    //vec3 emissive = texture(mat.emissive, TextCoord).rgb;
    
	vec3 dirColor = GetDirLight(dirLight, viewDir);
    // FragColor = mix(texture(uTexture0, TextCoord), texture(uTexture1, TextCoord), mixValue);
    //FragColor = vec4(ambientLight + diffuseLight + specular + emissive, 1);
	FragColor = vec4(dirColor, 1);
    //FragColor = texture(mat.diffuse, TextCoord);
} 

vec3 GetDirLight(LightDirectional light, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(-lightDir, Normal);

	vec3 ambientLight = light.ambient * texture(mat.diffuse, TextCoord).rgb;
	vec3 diffuseLight = max(dot(Normal, lightDir), 0.0) * light.diffuse * texture(mat.diffuse, TextCoord).rgb;
	vec3 specularLight = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess) * light.specular * texture(mat.specular, TextCoord).rgb;
	vec3 emissive = texture(mat.emissive, TextCoord).rgb;
	return ambientLight + diffuseLight + specularLight + emissive;
}