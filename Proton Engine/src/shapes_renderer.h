//
//  shapes_renderer.h
//  Proton Engine
//
//  Created by VibingCatt on 10/11/25.
//

#pragma once
#include <vector>
#include <string>

class Cube;

class Renderer {
public:
    static void RegisterCube(Cube* cube);
    static void RenderAll(unsigned int shaderProgram);
    static const std::vector<Cube*>& GetCubes() { return cubes; }
    
private:
    static std::vector<Cube*> cubes;
};
