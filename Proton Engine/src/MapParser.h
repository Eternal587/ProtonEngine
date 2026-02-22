//
//  MapParser.h
//  Proton Engine
//
//  Created by VibingCatt on 10/21/25.
//

#pragma once
#include "shapes.h"
#include <vector>
#include <string>

struct Object {
    std::string NAME;
    std::string TYPE_OF_OBJECT;
    float X, Y, Z;
    float W, H, D;
    float R, G, B;
    std::string PATH_TO_TEXTURE;
    float MIPMAP_LEVELX, MIPMAP_LEVELY, MIPMAP_LEVELZ;
    float SHINYNESS;
    float ROTATION_X, ROTATION_Y, ROTATION_Z;
};

void parse_map(const std::string map);
void save_map(const std::string loc);
void unloadMap();
