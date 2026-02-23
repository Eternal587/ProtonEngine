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

struct Hitbox {
    glm::vec3 position, dimensions, dir, axes[3], half_extents;
};

class Cube {
public:
    glm::vec3 last_pos;
    glm::vec3 last_dir;
    unsigned int VAO, VBO, EBO;
    glm::vec3 position;
    glm::vec3 dimensions;
    glm::vec3 color;
    std::string pathtotexture;
    unsigned int texture, diffuse_texture;
    glm::vec3 tiles;
    float shinyness;
    float degree_x, degree_y, degree_z;
    std::string name;
    Hitbox hitbox;
    bool toDelete;

    Cube(std::string cube_name, const glm::vec3& pos, const glm::vec3& dim, const glm::vec3& col, const std::string& path, glm::vec3 mipmap, float shine, float rotation_x, float rotation_y, float rotation_z);
    void Render(unsigned int shaderProgram);
    
    void deleteSelf();
    void reInitTexture();
private:
    void setupMesh();
    void recalculate_normals();
};

class Slope {
public:
    glm::vec3 last_pos;
    glm::vec3 last_dir;
    unsigned int VAO, VBO, EBO;
    glm::vec3 position;
    glm::vec3 dimensions;
    glm::vec3 color;
    std::string pathtotexture;
    unsigned int texture, diffuse_texture;
    glm::vec3 tiles;
    float shinyness;
    float degree_x, degree_y, degree_z;
    std::string name;
    Hitbox hitbox;
    bool toDelete;

    Slope(std::string slope_name, const glm::vec3& pos, const glm::vec3& dim, const glm::vec3& col, const std::string& path, glm::vec3 mipmap, float shine, float rotation_x, float rotation_y, float rotation_z);
    void Render(unsigned int shaderProgram);
    
    void deleteSelf();
private:
    void setupMesh();
    void recalculate_normals();
};

struct LightSource {
    glm::vec3 position;
    glm::vec3 color;
    
    LightSource(const glm::vec3& pos, const glm::vec3& col);
};

class Skybox {
    std::string pathtotexture;
    Skybox(std::string texture_path);
};

glm::vec3 get_normal(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3);

void RegisterHitbox(Hitbox hitbox);

std::vector<Hitbox> returnHitboxes();
void clearHitboxes();
/*
class Mesh {
    public:
        glm::vec3 position;
        std::vector<glm::vec3> points;
        glm::vec3 color;
        std::string pathtotexture;
        unsigned int texture, diffuse_texture;
        glm::vec3 tiles;
        float shinyness;
        float degree_x, degree_y, degree_z;
        std::string name;
        
        Mesh(std::string meshName, const glm::vec3 pos, std::vector<glm::vec3> vertices, glm::vec3 col, std::string texpath, glm::vec3 mipmap, float shine, float rotation_x, float rotation_y, float rotation_z);
        void Render(unsigned int shaderProgram);
    
    private:
    void SetupMesh();
    void calculateNormals();
};
*/
