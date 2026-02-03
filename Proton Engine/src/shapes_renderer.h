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
class Slope;
class Mesh;

class Renderer {
public:
    static void RegisterSlope(Slope* slope);
    static void RegisterCube(Cube* cube);
    static void RegisterMesh(Mesh* mesh);
    static void RenderAll(unsigned int shaderProgram);

private:
    static std::vector<Cube*> cubes;
    static std::vector<Slope*> slopes;
    static std::vector<Mesh*> meshs;
};
