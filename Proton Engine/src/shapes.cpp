//
//  shapes.cpp
//  Proton Engine
//
//  Created by VibingCatt on 10/11/25.
//


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shapes.h"
#include "shapes_renderer.h"
#include "glad.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Cube::Cube(std::string cube_name, const glm::vec3& pos, const glm::vec3& dim, const glm::vec3& col, const std::string& path, glm::vec3 mipmap, const float shine, const float rotation_x, const float rotation_y, const float rotation_z)
: position(pos), dimensions(dim), color(col), pathtotexture(path), tiles(mipmap), shinyness(shine), name(cube_name), degree_x(rotation_x), degree_y(rotation_y), degree_z(rotation_z)
{
    setupMesh();
    Renderer::RegisterCube(this); // auto-register itself
}

void Cube::setupMesh() {
    float w = dimensions.x;
    float h = dimensions.y;
    float d = dimensions.z;
    
    float tx = tiles.x;
    float ty = tiles.y;
    float tz = tiles.z;
    

    float verts[] = {
        
        // Back face (Z = 0)
        0, 0, 0,   color.r, color.g, color.b,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f, shinyness,
        w, 0, 0,   color.r, color.g, color.b,  tx, 0.0f, 0.0f, 0.0f, -1.0f, shinyness,
        w, h, 0,   color.r, color.g, color.b,  tx, ty, 0.0f, 0.0f, -1.0f, shinyness,
        0, h, 0,   color.r, color.g, color.b,  0.0f, ty, 0.0f, 0.0f, -1.0f, shinyness,
        
        // Front face (Z = d)
        0, 0, d,   color.r, color.g, color.b,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, shinyness,
        w, 0, d,   color.r, color.g, color.b,  tx, 0.0f, 0.0f, 0.0f, 1.0f, shinyness,
        w, h, d,   color.r, color.g, color.b,  tx, ty, 0.0f, 0.0f, 1.0f, shinyness,
        0, h, d,   color.r, color.g, color.b,  0.0f, ty, 0.0f, 0.0f, 1.0f, shinyness,

        // Left face (X = 0)
        0, 0, 0,   color.r, color.g, color.b,  0.0f, 0.0f,-1.0f, 0.0f, 0.0f, shinyness,
        0, 0, d,   color.r, color.g, color.b,  tz, 0.0f, -1.0f, 0.0f, 0.0f, shinyness,
        0, h, d,   color.r, color.g, color.b,  tz, ty, -1.0f, 0.0f, 0.0f, shinyness,
        0, h, 0,   color.r, color.g, color.b,  0.0f, ty, -1.0f, 0.0f, 0.0f, shinyness,

        // Right face (X = w)
        w, 0, 0,   color.r, color.g, color.b,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, shinyness,
        w, 0, d,   color.r, color.g, color.b,  tz, 0.0f, 1.0f, 0.0f, 0.0f, shinyness,
        w, h, d,   color.r, color.g, color.b,  tz, ty, 1.0f, 0.0f, 0.0f, shinyness,
        w, h, 0,   color.r, color.g, color.b,  0.0f, ty, 1.0f, 0.0f, 0.0f, shinyness,

        // Bottom face (Y = 0)
        0, 0, 0,   color.r, color.g, color.b,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, shinyness,
        w, 0, 0,   color.r, color.g, color.b,  tz, 0.0f, 0.0f, -1.0f, 0.0f, shinyness,
        w, 0, d,   color.r, color.g, color.b,  tz, tz, 0.0f, -1.0f, 0.0f, shinyness,
        0, 0, d,   color.r, color.g, color.b,  0.0f, tz, 0.0f, -1.0f, 0.0f, shinyness,

        // Top face (Y = h)
        0, h, 0,   color.r, color.g, color.b,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, shinyness,
        w, h, 0,   color.r, color.g, color.b,  tx, 0.0f, 0.0f, 1.0f, 0.0f, shinyness,
        w, h, d,   color.r, color.g, color.b,  tx, tz, 0.0f, 1.0f, 0.0f, shinyness,
        0, h, d,   color.r, color.g, color.b,  0.0f, tz, 0.0f, 1.0f, 0.0f, shinyness
    };


    unsigned int inds[] = {
        0,1,2,  2,3,0,        // back
        4,5,6,  6,7,4,        // front
        8,9,10, 10,11,8,      // left
        12,13,14, 14,15,12,   // right
        16,17,18, 18,19,16,   // bottom
        20,21,22, 22,23,20    // top
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);
    
    
    /// Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    /// Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    /// Texture Corrdinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    /// Normals
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
    
    /// Speccular Strength
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(4);
    
    
    stbi_set_flip_vertically_on_load(true);
    int width, height, nr_channels;
    int d_width, d_height, d_nr_channels;
    unsigned char *data = stbi_load(pathtotexture.c_str(), &width, &height, &nr_channels, 0);
    
    std::string diffuse_path = pathtotexture.substr(0, pathtotexture.find(".")) + "_diffuse" + (pathtotexture.substr(pathtotexture.find("."), pathtotexture.length() - 1));
    
    unsigned char *data_diffuse = stbi_load(diffuse_path.c_str(), &d_width, &d_height, &d_nr_channels, 0);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    /// glTexImage2D(Texture type, mipmap level, how we want OpenGL to store said textures, setting the width and height of the texture, (always 0: "legacy stuff"),  format, data type, imagedata)
    if (!data) {
    std::cerr << "Failed to load base texture: " << pathtotexture << std::endl;
    return;
}
    if(data) {
        GLenum format = (nr_channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load the texture\n";
    }
    
    glGenTextures(1, &diffuse_texture);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture);
    
    // wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    if(data_diffuse) {
        GLenum format = (d_nr_channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, d_width, d_height, 0, format, GL_UNSIGNED_BYTE, data_diffuse);

        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load the texture\n";
    }
    if (!data_diffuse) {
        std::cerr << "Failed to load base texture: " << diffuse_path << std::endl;
        diffuse_texture = texture;
    }
    
    stbi_image_free(data);
    stbi_image_free(data_diffuse);
    
    recalculate_normals();
    
}

void Cube::recalculate_normals() {
    float w = dimensions.x;
    float h = dimensions.y;
    float d = dimensions.z;
    
    float tx = tiles.x;
    float ty = tiles.y;
    float tz = tiles.z;
    
    glm::vec3 normals[6] {{0.0f, 0.0f, -1.0f,}, {0.0f, 0.0f, 1.0f,}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
    
    glm::vec3 rotate_x = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 rotate_y = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 rotate_z = glm::vec3(0.0f, 0.0f, 1.0f);
    
    glm::mat4 rotate_matrix_x = glm::mat4(1.0f);
    glm::mat4 rotate_matrix_y = glm::mat4(1.0f);
    glm::mat4 rotate_matrix_z = glm::mat4(1.0f);
    
    rotate_matrix_x = glm::rotate(rotate_matrix_x, glm::radians(degree_x), rotate_x);
    rotate_matrix_y = glm::rotate(rotate_matrix_y, glm::radians(degree_y), rotate_y);
    rotate_matrix_z = glm::rotate(rotate_matrix_z, glm::radians(degree_z), rotate_z);

    glm::mat3 normal_matrix_x = glm::mat3(rotate_matrix_x);
    glm::mat3 normal_matrix_y = glm::mat3(rotate_matrix_y);
    glm::mat3 normal_matrix_z = glm::mat3(rotate_matrix_z);
    
    for(int i=0; i < 6; i++) {
        glm::vec4 temp_vector = rotate_matrix_x * rotate_matrix_y * rotate_matrix_z * glm::vec4(normals[i], 1.0f);
        normals[i] = glm::normalize(glm::vec3(temp_vector));
    }
    
    float verts[] = {
        
        // Back face (Z = 0)
        0, 0, 0,   color.r, color.g, color.b,  0.0f, 0.0f, normals[0].x, normals[0].y, normals[0].z, shinyness,
        w, 0, 0,   color.r, color.g, color.b,  tx, 0.0f, normals[0].x, normals[0].y, normals[0].z, shinyness,
        w, h, 0,   color.r, color.g, color.b,  tx, ty, normals[0].x, normals[0].y, normals[0].z, shinyness,
        0, h, 0,   color.r, color.g, color.b,  0.0f, ty, normals[0].x, normals[0].y, normals[0].z, shinyness,
        
        // Front face (Z = d)
        0, 0, d,   color.r, color.g, color.b,  0.0f, 0.0f, normals[1].x, normals[1].y, normals[1].z, shinyness,
        w, 0, d,   color.r, color.g, color.b,  tx, 0.0f, normals[1].x, normals[1].y, normals[1].z, shinyness,
        w, h, d,   color.r, color.g, color.b,  tx, ty, normals[1].x, normals[1].y, normals[1].z, shinyness,
        0, h, d,   color.r, color.g, color.b,  0.0f, ty, normals[1].x, normals[1].y, normals[1].z, shinyness,

        // Left face (X = 0)
        0, 0, 0,   color.r, color.g, color.b,  0.0f, 0.0f, normals[2].x, normals[2].y, normals[2].z, shinyness,
        0, 0, d,   color.r, color.g, color.b,  tz, 0.0f, normals[2].x, normals[2].y, normals[2].z, shinyness,
        0, h, d,   color.r, color.g, color.b,  tz, ty, normals[2].x, normals[2].y, normals[2].z, shinyness,
        0, h, 0,   color.r, color.g, color.b,  0.0f, ty, normals[2].x, normals[2].y, normals[2].z, shinyness,

        // Right face (X = w)
        w, 0, 0,   color.r, color.g, color.b,  0.0f, 0.0f, normals[3].x, normals[3].y, normals[3].z,shinyness,
        w, 0, d,   color.r, color.g, color.b,  tz, 0.0f, normals[3].x, normals[3].y, normals[3].z, shinyness,
        w, h, d,   color.r, color.g, color.b,  tz, ty, normals[3].x, normals[3].y, normals[3].z, shinyness,
        w, h, 0,   color.r, color.g, color.b,  0.0f, ty, normals[3].x, normals[3].y, normals[3].z, shinyness,

        // Bottom face (Y = 0)
        0, 0, 0,   color.r, color.g, color.b,  0.0f, 0.0f, normals[4].x, normals[4].y, normals[4].z, shinyness,
        w, 0, 0,   color.r, color.g, color.b,  tz, 0.0f, normals[4].x, normals[4].y, normals[4].z, shinyness,
        w, 0, d,   color.r, color.g, color.b,  tz, tz, normals[4].x, normals[4].y, normals[4].z, shinyness,
        0, 0, d,   color.r, color.g, color.b,  0.0f, tz, normals[4].x, normals[4].y, normals[4].z, shinyness,

        // Top face (Y = h)
        0, h, 0,   color.r, color.g, color.b,  0.0f, 0.0f, normals[5].x, normals[5].y, normals[5].z, shinyness,
        w, h, 0,   color.r, color.g, color.b,  tx, 0.0f, normals[5].x, normals[5].y, normals[5].z, shinyness,
        w, h, d,   color.r, color.g, color.b,  tx, tz, normals[5].x, normals[5].y, normals[5].z, shinyness,
        0, h, d,   color.r, color.g, color.b,  0.0f, tz, normals[5].x, normals[5].y, normals[5].z, shinyness
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
}

void Cube::Render(unsigned int shaderProgram) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        
        /// Recalculating the Angle and normals each frame
        model = glm::rotate(model, glm::radians(degree_x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(degree_y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(degree_z), glm::vec3(0.0f, 0.0f, 1.0f));
        recalculate_normals();
    
    
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuse_texture);
        glUniform1i(glGetUniformLocation(shaderProgram, "diffuseTexture"), 1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

Slope::Slope(std::string slope_name, const glm::vec3& pos, const glm::vec3& dim, const glm::vec3& col, const std::string& path, glm::vec3 mipmap, const float shine, const float rotation_x, const float rotation_y, const float rotation_z)
: position(pos), dimensions(dim), color(col), pathtotexture(path), tiles(mipmap), shinyness(shine), name(slope_name), degree_x(rotation_x), degree_y(rotation_y), degree_z(rotation_z)
{
    setupMesh();
    Renderer::RegisterSlope(this); // auto-register itself
}

void Slope::setupMesh() {
    float w = dimensions.x;
    float h = dimensions.y;
    float d = dimensions.z;
    
    float tx = tiles.x;
    float ty = tiles.y;
    float tz = tiles.z;
    

    float verts[] = {
        // Bottom face (y = 0)
        0, 0, 0,    color.r, color.g, color.b,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f, shinyness,
        w, 0, 0,    color.r, color.g, color.b,   1.0f, 0.0f,     0.0f, -1.0f, 0.0f, shinyness,
        w, 0, d,    color.r, color.g, color.b,   1.0f, 1.0f,     0.0f, -1.0f, 0.0f, shinyness,
        0, 0, d,    color.r, color.g, color.b,   0.0f, 1.0f,     0.0f, -1.0f, 0.0f, shinyness,

        // Back face (Z = 0)
        0, 0, 0,    color.r, color.g, color.b,   0.0f, 0.0f,     0.0f, 0.0f, -1.0f, shinyness,
        w, 0, 0,    color.r, color.g, color.b,   1.0f, 0.0f,     0.0f, 0.0f, -1.0f, shinyness,
        w, h, 0,    color.r, color.g, color.b,   1.0f, 1.0f,     0.0f, 0.0f, -1.0f, shinyness,
        0, h, 0,    color.r, color.g, color.b,   0.0f, 1.0f,     0.0f, 0.0f, -1.0f, shinyness,

        // Front slope face (from back top to front bottom)
        0, h, 0,    color.r, color.g, color.b,   0.0f, 0.0f,     0.0f, 0.707f, 0.707f, shinyness,
        w, h, 0,    color.r, color.g, color.b,   1.0f, 0.0f,     0.0f, 0.707f, 0.707f, shinyness,
        w, 0, d,    color.r, color.g, color.b,   1.0f, 1.0f,     0.0f, 0.707f, 0.707f, shinyness,
        0, 0, d,    color.r, color.g, color.b,   0.0f, 1.0f,     0.0f, 0.707f, 0.707f, shinyness,

        // Left face
        0, 0, 0,    color.r, color.g, color.b,   0.0f, 0.0f,    -1.0f, 0.0f, 0.0f, shinyness,
        0, h, 0,    color.r, color.g, color.b,   1.0f, 0.0f,    -1.0f, 0.0f, 0.0f, shinyness,
        0, 0, d,    color.r, color.g, color.b,   1.0f, 1.0f,    -1.0f, 0.0f, 0.0f, shinyness,

        // Right face
        w, 0, 0,    color.r, color.g, color.b,   0.0f, 0.0f,     1.0f, 0.0f, 0.0f, shinyness,
        w, h, 0,    color.r, color.g, color.b,   1.0f, 0.0f,     1.0f, 0.0f, 0.0f, shinyness,
        w, 0, d,    color.r, color.g, color.b,   1.0f, 1.0f,     1.0f, 0.0f, 0.0f, shinyness
    };

    unsigned int inds[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14,
        15, 16, 17
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);
    
    
    /// Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    /// Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    /// Texture Corrdinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    /// Normals
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
    
    /// Speccular Strength
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(4);
    
    
    stbi_set_flip_vertically_on_load(true);
    int width, height, nr_channels;
    int d_width, d_height, d_nr_channels;
    unsigned char *data = stbi_load(pathtotexture.c_str(), &width, &height, &nr_channels, 0);
    
    std::string diffuse_path = pathtotexture.substr(0, pathtotexture.find(".")) + "_diffuse" + (pathtotexture.substr(pathtotexture.find("."), pathtotexture.length() - 1));
    
    unsigned char *data_diffuse = stbi_load(diffuse_path.c_str(), &d_width, &d_height, &d_nr_channels, 0);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    /// glTexImage2D(Texture type, mipmap level, how we want OpenGL to store said textures, setting the width and height of the texture, (always 0: "legacy stuff"),  format, data type, imagedata)
    if (!data) {
        std::cerr << "Failed to load base texture: " << pathtotexture << std::endl;
        return;
    }
    
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load the texture\n";
    }
    
    glGenTextures(1, &diffuse_texture);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture);

    // wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    if(data_diffuse) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d_width, d_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_diffuse);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load the texture\n";
    }
    
    if (!data_diffuse) {
        std::cerr << "Failed to load base texture: " << diffuse_path << std::endl;
        diffuse_texture = texture;
    }
    
    stbi_image_free(data);
    stbi_image_free(data_diffuse);
}

void Slope::recalculate_normals() {
    float w = dimensions.x;
    float h = dimensions.y;
    float d = dimensions.z;
    
    float tx = tiles.x;
    float ty = tiles.y;
    float tz = tiles.z;
    
    glm::vec3 normals[6] {{0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.707f, 0.707f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};
    
    glm::vec3 rotate_x = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 rotate_y = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 rotate_z = glm::vec3(0.0f, 0.0f, 1.0f);
    
    glm::mat4 rotate_matrix_x = glm::mat4(1.0f);
    glm::mat4 rotate_matrix_y = glm::mat4(1.0f);
    glm::mat4 rotate_matrix_z = glm::mat4(1.0f);
    
    rotate_matrix_x = glm::rotate(rotate_matrix_x, glm::radians(degree_x), rotate_x);
    rotate_matrix_y = glm::rotate(rotate_matrix_y, glm::radians(degree_y), rotate_y);
    rotate_matrix_z = glm::rotate(rotate_matrix_z, glm::radians(degree_z), rotate_z);

    glm::mat3 normal_matrix_x = glm::mat3(rotate_matrix_x);
    glm::mat3 normal_matrix_y = glm::mat3(rotate_matrix_y);
    glm::mat3 normal_matrix_z = glm::mat3(rotate_matrix_z);
    
    for(int i=0; i < 6; i++) {
        glm::vec4 temp_vector = rotate_matrix_x * rotate_matrix_y * rotate_matrix_z * glm::vec4(normals[i], 1.0f);
        normals[i] = glm::normalize(glm::vec3(temp_vector));
    }
    
    float verts[] = {
        // Bottom face (y = 0)
        0, 0, 0,    color.r, color.g, color.b,   0.0f, 0.0f,     normals[0].x, normals[0].y, normals[0].z, shinyness,
        w, 0, 0,    color.r, color.g, color.b,   1.0f, 0.0f,     normals[0].x, normals[0].y, normals[0].z, shinyness,
        w, 0, d,    color.r, color.g, color.b,   1.0f, 1.0f,     normals[0].x, normals[0].y, normals[0].z, shinyness,
        0, 0, d,    color.r, color.g, color.b,   0.0f, 1.0f,     normals[0].x, normals[0].y, normals[0].z, shinyness,

        // Back face (Z = 0)
        0, 0, 0,    color.r, color.g, color.b,   0.0f, 0.0f,     normals[1].x, normals[1].y, normals[1].z, shinyness,
        w, 0, 0,    color.r, color.g, color.b,   1.0f, 0.0f,     normals[1].x, normals[1].y, normals[1].z, shinyness,
        w, h, 0,    color.r, color.g, color.b,   1.0f, 1.0f,     normals[1].x, normals[1].y, normals[1].z, shinyness,
        0, h, 0,    color.r, color.g, color.b,   0.0f, 1.0f,     normals[1].x, normals[1].y, normals[1].z, shinyness,

        // Front slope face (from back top to front bottom)
        0, h, 0,    color.r, color.g, color.b,   0.0f, 0.0f,     normals[2].x, normals[2].y, normals[2].z, shinyness,
        w, h, 0,    color.r, color.g, color.b,   1.0f, 0.0f,     normals[2].x, normals[2].y, normals[2].z, shinyness,
        w, 0, d,    color.r, color.g, color.b,   1.0f, 1.0f,     normals[2].x, normals[2].y, normals[2].z, shinyness,
        0, 0, d,    color.r, color.g, color.b,   0.0f, 1.0f,     normals[2].x, normals[2].y, normals[2].z, shinyness,

        // Left face
        0, 0, 0,    color.r, color.g, color.b,   0.0f, 0.0f,    normals[3].x, normals[3].y, normals[3].z, shinyness,
        0, h, 0,    color.r, color.g, color.b,   1.0f, 0.0f,    normals[3].x, normals[3].y, normals[3].z, shinyness,
        0, 0, d,    color.r, color.g, color.b,   1.0f, 1.0f,    normals[3].x, normals[3].y, normals[3].z, shinyness,

        // Right face
        w, 0, 0,    color.r, color.g, color.b,   0.0f, 0.0f,     normals[4].x, normals[4].y, normals[4].z, shinyness,
        w, h, 0,    color.r, color.g, color.b,   1.0f, 0.0f,     normals[4].x, normals[4].y, normals[4].z, shinyness,
        w, 0, d,    color.r, color.g, color.b,   1.0f, 1.0f,     normals[4].x, normals[4].y, normals[4].z, shinyness
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
}

void Slope::Render(unsigned int shaderProgram) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        
        /// Recalculating the Angle and normals each frame
        model = glm::rotate(model, glm::radians(degree_x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(degree_y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(degree_z), glm::vec3(0.0f, 0.0f, 1.0f));
        recalculate_normals();
    
    
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuse_texture);
        glUniform1i(glGetUniformLocation(shaderProgram, "diffuseTexture"), 1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
}


glm::vec3 get_normal(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3) {
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    
    return glm::normalize(glm::cross(edge1, edge2));
}
