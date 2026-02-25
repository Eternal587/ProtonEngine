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
class LightSource;
class Skybox;

class Renderer {
public:
    static void RegisterSlope(Slope* slope);
    static void RegisterCube(Cube* cube);
    static void RegisterMesh(Mesh* mesh);
    static void RegisterLight(LightSource* light);
    static void setSkyBox(Skybox* skybox);
    static void RenderAll(unsigned int shaderProgram);
    static void RenderSkybox(unsigned int shader);
    
    static std::vector<Cube*> returnCubes();
    static std::vector<Slope*> returnSlopes();
    static std::vector<LightSource*> returnLights();
    static void deleteCube(int index);
    static void deleteSlope(int index);
    static void deleteLight(int index);
    static void ClearAll();
    
    static Skybox* skybox;

private:
    static std::vector<Cube*> cubes;
    static std::vector<Slope*> slopes;
    static std::vector<Mesh*> meshs;
    static std::vector<LightSource*> lightsources;
    
};

