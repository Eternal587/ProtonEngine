//
//  shapes.h
//  Proton Engine
//
//  Created by VibingCatt on 10/11/25.
//

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Cube {
public:
    unsigned int VAO, VBO, EBO;
    glm::vec3 position;
    glm::vec3 dimensions;
    glm::vec3 color;
    std::string pathtotexture;
    unsigned int texture;
    glm::vec3 tiles;
    float shinyness;
    std::string name;

    Cube(const glm::vec3& pos, const glm::vec3& dim, const glm::vec3& col, const std::string& path, glm::vec3 mipmap, float shine);
    void Render(unsigned int shaderProgram);

private:
    void setupMesh();
};

struct LightSource {
    glm::vec3 position;
    glm::vec3 color;
    
    LightSource(const glm::vec3& pos, const glm::vec3& col);
};
