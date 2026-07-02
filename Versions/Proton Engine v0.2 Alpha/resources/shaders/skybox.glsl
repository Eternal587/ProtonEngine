#shader vertex
#version 330 core
layout (location = 0) in vec2 aTexCoord;

out vec2 TexCoord;

layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0, 1.0);
}

#shader fragment
#version 330 core

uniform sampler2D texture;
in vec2 TexCoord;

out vec4 FragColor;
void main() {
    FragColor = vec4(0.2, 0.7, 0.3, 1.0);
}
