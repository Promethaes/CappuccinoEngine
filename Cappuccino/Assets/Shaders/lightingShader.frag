#version 420 core
out vec4 FragColor;
  
  
 struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};  
  
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  


#define NR_POINT_LIGHTS 4  

uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; 
in vec3 Normal; 
in vec3 FragPos; 
in vec2 TexCoords;


//functions
vec3 calculateDirectionalLight(DirectionalLight dLight,vec3 normal,vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 

void main()
{

vec3 result = vec3(1.0);
vec3 norm = normalize(Normal);
vec3 viewDir = normalize(viewPos - FragPos);

result = calculateDirectionalLight(dirLight, norm, viewDir);

for(int i = 0;i < NR_POINT_LIGHTS;i++)
  result+=calculatePointLight(pointLights[i],norm,FragPos,viewDir);

FragColor = vec4(result, 1.0);

}

vec3 calculateDirectionalLight(DirectionalLight dLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dLight.direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // combine results
    vec3 ambient  = dLight.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = dLight.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = dLight.specular * spec * vec3(texture(material.specular, TexCoords));
    
    return (ambient + diffuse + specular);
}  

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos,vec3 viewDir){

  
   vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}