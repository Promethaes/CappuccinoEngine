#version 420 core
out vec4 FragColor;
  
  
 struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};  
  
struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  


uniform DirectionalLight dirLight;
uniform Material material;
uniform vec3 viewPos;
in vec3 Normal; 
in vec3 FragPos; 
in vec2 TexCoords;


//functions
vec3 calculateDirectionalLight(DirectionalLight dLight,vec3 normal,vec3 viewDir);

void main()
{

vec3 result = vec3(1.0);
vec3 norm = normalize(Normal);
vec3 viewDir = normalize(viewPos - FragPos);

result = calculateDirectionalLight(dirLight, norm, viewDir);

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
