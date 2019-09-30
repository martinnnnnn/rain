// #version 420 core

// out vec4 FragColor;

// // in VS_OUT
// // {
// //     vec3 Position;
// //     vec3 Normal;
// // } fs_in;

// in vec3 Normal;

// // uniform vec3 lightDirection;
// // uniform vec3 lightDiff;

// void main()
// {
    
//     // vec3 lightDir = normalize(-lightDirection);
//     // vec3 diffuseLight = max(dot(fs_in.Normal, lightDir), 0.0) * lightDiff;
//     // FragColor =  vec4(1.0f, 0.5f, 0.2f, 1.0f) * vec4(diffuseLight, 1.0);
//     FragColor = vec4(Normal, 1);
// }



#version 420 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 

