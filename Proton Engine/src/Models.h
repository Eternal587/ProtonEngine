//
//  Models.h
//  Proton Engine
//
//  Created by VibingCatt on 10/13/25.
//
#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 texture_coords;
  glm::vec3 normal;
  float shinyness;
};

struct Texture {
  unsigned int id;
  std::string type;
};

class Mesh {
  public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Init Function
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<Texture> textures);

    void Render(unsigned int shader_program);
  private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};
