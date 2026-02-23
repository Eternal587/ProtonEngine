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

/*void Renderer::RegisterMesh(Mesh* mesh) {
    meshs.push_back(mesh);
    std::cout << "Registered Mesh\n";
}*/

void Renderer::RenderAll(unsigned int shaderProgram) {
    for (Cube* cube : cubes)
        cube->Render(shaderProgram);
    for (Slope* slope : slopes)
        slope->Render(shaderProgram);
    //for (Mesh* mesh : meshs)
    //  mesh->Render(shaderProgram);
}

std::vector<Cube*> Renderer::returnCubes() {
    return Renderer::cubes;
}

std::vector<Slope*> Renderer::returnSlopes() {
    return Renderer::slopes;
}
std::vector<LightSource*> Renderer::returnLights() {
    return Renderer::lightSources;
}

void Renderer::deleteCube(int index) {
    cubes[index]->deleteSelf();
    cubes.erase(cubes.begin() + index);
}

void Renderer::deleteSlope(int index) {
    slopes[index]->deleteSelf();
    slopes.erase(slopes.begin() + index);
}

void Renderer::ClearAll() {
    for(auto c : cubes) delete c;
    cubes.clear();
    for(auto s : slopes) delete s;
    slopes.clear();
    void clearHitboxes();
    
}
