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
#include <iostream>

std::vector<Cube*> Renderer::cubes;
std::vector<Slope*> Renderer::slopes;

void Renderer::RegisterCube(Cube* cube) {
    cubes.push_back(cube);
    std::cout << "Registered Cube\n";
}

void Renderer::RegisterSlope(Slope* slope) {
    slopes.push_back(slope);
    std::cout << "Registered Slope\n";
}

void Renderer::RenderAll(unsigned int shaderProgram) {
    for (Cube* cube : cubes)
        cube->Render(shaderProgram);
    for (Slope* slope : slopes)
        slope->Render(shaderProgram);
}

