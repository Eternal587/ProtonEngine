//
//  shapes_renderer.cpp
//  Proton Engine
//
//  Created by VibingCatt on 10/11/25.
//

#include "shapes_renderer.h"
#include "shapes.h"
#include "glad.h"
#include <glm/glm.hpp>

std::vector<Cube*> Renderer::cubes;

void Renderer::RegisterCube(Cube* cube) {
    cubes.push_back(cube);
}

void Renderer::RenderAll(unsigned int shaderProgram) {
    for (Cube* cube : cubes)
        cube->Render(shaderProgram);
}




