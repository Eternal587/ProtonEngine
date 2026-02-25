//
//  shaderManager.cpp
//  Proton Engine
//
//  Created by VibingCatt on 2/25/26.
//
#include "shaderManager.h"

const char* fallback_vertex = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0, 1.0);
}
)";

const char* fallback_fragment = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.2, 0.7, 0.3, 1.0);
}
)";
