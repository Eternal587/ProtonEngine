#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 FragPos
out vec2 texCoord

void main() {
  gl_Position = vec4(aPos, 0.0f, 1.0f);

  FragPos = vec3(aPos, 0.0f);
  TexCoord = aTexCoord;
}

#shader fragment
#version 330 core

in vec3 FragPos

unifor
