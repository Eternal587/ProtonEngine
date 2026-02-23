#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in float aShinyness;

out vec3 vertexColor;
out vec2 TexCoord;
out vec3 normal;
out float specularStrength;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 alight_sources;
uniform vec3 alight_color;

out vec3 light_sources;
out vec3 light_color;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
    
    normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    specularStrength = aShinyness;
}


#shader fragment
#version 330 core

struct LightSource {
    vec3 position;
    vec3 color;
};
#define MAX_LIGHTS 30
uniform int NRLightSources;
uniform LightSource lightSources[30];

uniform vec3 viewPos;
out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;
in float specularStrength;

in vec3 normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D diffuseTexture;
uniform sampler2D speccularTexture;

void main()
{
    float ambient_strength = 0.4f;
    vec4 texColor = texture(texture1, TexCoord);
    
    if (texColor.a < 0.1)
            discard;
    
    vec3 norm = normalize(normal);
    vec3 result = vec3(0.0f);
    
    int numLights = clamp(NRLightSources, 0, MAX_LIGHTS);
    
    for(int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lightSources[i].position - FragPos);
        
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightSources[i].color * vec3(texture(diffuseTexture, TexCoord));
        
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightSources[i].color;
        
        result += (diffuse + specular);
    }
    
    vec3 finalColor = vertexColor * (vec3(ambient_strength) + result);
    
    FragColor = texColor * vec4(finalColor, 1.0);
}
