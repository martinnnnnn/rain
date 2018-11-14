// #version 420 core
// out vec4 FragColor;

// #define DIRECTIONAL_LIGHT 0
// #define POINT_LIGHT 1
// #define SPOT_LIGHT 2

// in VS_OUT
// {
//     vec3 Position;
//     vec3 Normal;
//     vec2 TextCoord;
// } fs_in;

// struct Material
// {
//     sampler2D diffuse;
//     sampler2D specular;
//     sampler2D emissive;
//     float shininess;
// };

// struct Light
// {
// 	int type;

// 	vec3 position;
// 	vec3 direction;
// 	vec3 ambient;
// 	vec3 diffuse;
// 	vec3 specular;
	
// 	float constant;
// 	float linear;
// 	float quadratic;

// 	float cutOff;
// 	float outerCutOff;
// };

// uniform vec3 viewPos;

// uniform Material mat;

// uniform Light light1;
// uniform Light light2;
// uniform Light light3;

// vec3 CalcLight(Light light);
// vec4 GetLinearizedDepth();

// void main()
// {
//     vec3 I = normalize(fs_in.Position - viewPos);
//     vec3 R = reflect(I, normalize(fs_in.Normal));


// 	vec3 outColor = CalcLight(light1);
// 	outColor += CalcLight(light2);
// 	outColor += CalcLight(light3);
//     //outColor += texture(skybox, R).rgb * texture(mat.specular, fs_in.TextCoord).rgb;
//     float ratio = 1.00 / 2.42;
//     I = normalize(fs_in.Position - viewPos);
//     R = refract(I, normalize(fs_in.Normal), ratio);
//     //outColor += texture(skybox, R).rgb, 1.0 * (-texture(mat.specular, fs_in.TextCoord).rgb);

// 	FragColor = vec4(outColor, 1);
// } 

// vec3 CalcLight(Light light)
// {
// 	vec3 viewDir = normalize(viewPos - fs_in.Position);
// 	vec3 lightDir = vec3(0, -1, 0);

// 	float attenuation = 1;
// 	float intensity = 1;
// 	float distance = 0;

// 	switch (light.type)
// 	{
// 	case DIRECTIONAL_LIGHT:
// 		lightDir = normalize(-light.direction);
// 		break;

// 	case POINT_LIGHT:
// 		lightDir = normalize(light.position - fs_in.Position);
// 		distance = length(light.position - fs_in.Position);
// 		attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
// 		break;
		
// 	case SPOT_LIGHT:
// 		lightDir = normalize(light.position - fs_in.Position);
// 		distance = length(light.position - fs_in.Position);
// 		attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
// 		float theta 	= dot(lightDir, normalize(-light.direction));
// 		float epsilon   = light.cutOff - light.outerCutOff;
// 		intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
// 		break;
// 	}
// 	vec3 reflectDir = reflect(-lightDir, fs_in.Normal);
// 	vec3 sampledDiffuse = texture(mat.diffuse, fs_in.TextCoord).rgb;

// 	vec3 ambientLight = light.ambient * sampledDiffuse;
// 	vec3 diffuseLight = max(dot(fs_in.Normal, lightDir), 0.0) * light.diffuse * sampledDiffuse;
// 	vec3 specularLight = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess) * light.specular * texture(mat.specular, fs_in.TextCoord).rgb;
// 	vec3 emissiveLight = texture(mat.emissive, fs_in.TextCoord).rgb;

// 	ambientLight *= attenuation;
// 	diffuseLight *= attenuation * intensity;
// 	specularLight *= attenuation * intensity;
// 	emissiveLight *= attenuation * intensity;	

// 	return ambientLight + diffuseLight + specularLight /*+ emissiveLight*/;
// }

#version 330 core


out vec4 FragColor;

in VS_OUT
{
    vec3 Position;
    vec3 Normal;
    vec2 TextCoord;
} fs_in;

uniform vec3 lightDirection;
uniform vec3 lightDiff;

void main()
{
    vec3 lightDir = normalize(-lightDirection);
    vec3 diffuseLight = max(dot(fs_in.Normal, lightDir), 0.0) * lightDiff;
    FragColor =  vec4(1.0f, 0.5f, 0.2f, 1.0f) * vec4(diffuseLight, 1.0);
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f) + max(dot(fs_in.Normal, normalize(direction)), 0.0) * lightDiff;
    // FragColor = max(dot(fs_in.Normal, normalize(-lightDirection)), 0.0) * lightDiff;
}