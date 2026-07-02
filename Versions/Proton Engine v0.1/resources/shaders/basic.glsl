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
    
    light_sources = alight_sources;
    light_color = alight_color;
    normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    specularStrength = aShinyness;
}


#shader fragment
#version 330 core
uniform vec3 viewPos;
out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;
in float specularStrength;

in vec3 normal;
in vec3 FragPos;
in vec3 light_sources;
in vec3 light_color;

uniform sampler2D texture1;

vec3 norm = normalize(normal);
vec3 lightDir = normalize(light_sources - FragPos);


float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * light_color;

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);

float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * light_color;

void main()
{
    float ambient_strength = 0.4f;
    vec4 texColor = texture(texture1, TexCoord);
    
    FragColor = texColor * vec4(vertexColor * (ambient_strength + diffuse + specular), 1.0);
}


