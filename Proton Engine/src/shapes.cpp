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


Cube::Cube(const glm::vec3& pos, const glm::vec3& dim, const glm::vec3& col, const std::string& path, glm::vec3 mipmap, const float shine)
    : position(pos), dimensions(dim), color(col), pathtotexture(path), tiles(mipmap), shinyness(shine)
{
    setupMesh();
    Renderer::RegisterCube(this); // auto-register itself
}

void Cube::setupMesh() {
    /// Getting the Dimensions and pos of the cube
    float w = dimensions.x;
    float h = dimensions.y;
    float d = dimensions.z;

    /// Getting the Mipmap for each side
    float tx = tiles.x;
    float ty = tiles.y;
    float tz = tiles.z;
    
    /// (Pos, Color, Tex Corrdinates, Normal Vector, Speccular Strength)
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

    /// Indices (always the same for cubes)
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
    
    /// Telling stbi to flip the images because by default the images are upside down
    stbi_set_flip_vertically_on_load(true);
    int width, height, nr_channels;

    /// Setting up the texture data
    unsigned char *data = stbi_load(pathtotexture.c_str(), &width, &height, &nr_channels, 0);

    /// Generating the textures glGenTextures(amount, texture)
    glGenTextures(1, &texture);
    /// Binding the texture glBindTexture(texture_type, texture)
    glBindTexture(GL_TEXTURE_2D, texture);

    // wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    /// glTexImage2D(Texture type, mipmap level, how we want OpenGL to store said textures, setting the width and height of the texture, (always 0: "legacy stuff"),  format, data type, imagedata)
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load the texture\n";
    }

    /// Freeing up the data
    stbi_image_free(data);
}

void Cube::Render(unsigned int shaderProgram) {
        /// Modifing the position of the cube from 0, 0, 0 to its actual specified posiion
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        /// Activating and rebinding the texture the submiting it to the shader to render
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

        /// Rebinding the VAO and telling OpenGL to draw call of the Triangles on the cube
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
