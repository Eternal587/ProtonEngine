#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexColor;
out vec2 TexCoord;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}


#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;
in vec3 FragPos;

uniform sampler2D texture1;

void main() {
    if (texColor.a < 0.1)
            discard;
    
    vec4 texColor = texture(texture1, TexCoord);

    FragColor = vec4(texColor, 1.0);
}
