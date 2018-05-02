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

struct Light
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

uniform vec3 viewPos;

uniform Material mat;
uniform Light dirLight;
uniform Light pointLight;
uniform Light spotLight;

uniform samplerCube skybox;

vec3 GetDirLight(Light light);
vec3 GetPointLight(Light light);
vec3 GetSpotLight(Light light);
vec4 GetLinearizedDepth();


void main()
{
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));

	vec3 outColor = GetDirLight(dirLight);
	outColor += GetPointLight(pointLight);
	outColor += GetSpotLight(spotLight);
    outColor += texture(skybox, R).rgb * texture(mat.specular, TextCoord).rgb;
	FragColor = vec4(outColor, 1);
    
} 

vec3 GetDirLight(Light light)
{
    vec3 viewDir = normalize(viewPos - Position);
	vec3 lightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(-lightDir, Normal);

	vec3 ambientLight = light.ambient * texture(mat.diffuse, TextCoord).rgb;
	vec3 diffuseLight = max(dot(Normal, lightDir), 0.0) * light.diffuse * texture(mat.diffuse, TextCoord).rgb;
	vec3 specularLight = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess) * light.specular * texture(mat.specular, TextCoord).rgb;
	vec3 emissive = texture(mat.emissive, TextCoord).rgb;
	return ambientLight + diffuseLight + specularLight;
}

vec3 GetPointLight(Light light)
{
	float distance = length(light.position - Position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	vec3 viewDir = normalize(viewPos - Position);
	vec3 lightDir = normalize(light.position - Position);
	vec3 reflectDir = reflect(-lightDir, Normal);

	vec3 ambientLight = light.ambient * texture(mat.diffuse, TextCoord).rgb;
	vec3 diffuseLight = max(dot(Normal, lightDir), 0.0) * light.diffuse * texture(mat.diffuse, TextCoord).rgb;
	vec3 specularLight = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess) * light.specular * texture(mat.specular, TextCoord).rgb;
	vec3 emissiveLight = texture(mat.emissive, TextCoord).rgb;

	ambientLight *= attenuation;
	diffuseLight *= attenuation;
	specularLight *= attenuation;
	emissiveLight *= attenuation;

	return ambientLight + diffuseLight + specularLight;
}

vec3 GetSpotLight(Light light)
{
	float distance = length(light.position - Position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	vec3 viewDir = normalize(viewPos - Position);
	vec3 lightDir = normalize(light.position - Position);
	vec3 reflectDir = reflect(-lightDir, Normal);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambientLight = light.ambient * texture(mat.diffuse, TextCoord).rgb;
	vec3 diffuseLight = max(dot(Normal, lightDir), 0.0) * light.diffuse * texture(mat.diffuse, TextCoord).rgb;
	vec3 specularLight = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess) * light.specular * texture(mat.specular, TextCoord).rgb;
	vec3 emissiveLight = texture(mat.emissive, TextCoord).rgb;

	ambientLight *= attenuation;
	diffuseLight *= attenuation * intensity;
	specularLight *= attenuation * intensity;
	emissiveLight *= attenuation * intensity;	
		
	return ambientLight + diffuseLight + specularLight;
}

vec4 GetLinearizedDepth()
{
	float near = 0.1;
	float far = 10.0;
	float z = gl_FragCoord.z * 2.0 - 1.0; 
	float linearDepth = (2.0 * near * far) / (far + near - z * (far - near));
	float depth = linearDepth / far;

	return vec4(vec3(depth), 1);
}