//
//  MapParser.cpp
//  Proton Engine
//
//  Created by VibingCatt on 10/23/25.
//

#include "MapParser.h"
#include "shapes_renderer.h"
#include "shapes.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <mach-o/dyld.h>
// #include <windows.h>
#include "json.hpp"

using json = nlohmann::json;

std::string getExePath() {
    char pathBuffer[1024];
    uint32_t size = sizeof(pathBuffer);
    if (_NSGetExecutablePath(pathBuffer, &size) != 0) {
        // Buffer too small; allocate dynamically if you want
        std::cerr << "Path buffer too small\n";
        return "";
    }
    return std::string(pathBuffer);
}

/// Windows Version Of GetExe
/*
std::string getExePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string path = std::string(buffer);
    path.erase(path.end() - 28, path.end());
    return path;
} */

const bool debug = true;

void parse_map(std::string map) {
    std::vector<Cube*> Cubes;
    std::string filepath = getExePath() + "/resources/maps/" + map;
    
    if (debug == true) {
        filepath = "/Users/vibingcatt/Documents/GitHub/ProtonEngine/Proton Engine/src/resources/maps/" + map;
    }
    
    /// Specifies the File your accessing
    std::ifstream stream(filepath);
    json data;
    stream >> data;
    
    std::vector<Object> objects;

    for (auto& item : data) {
        if (item["TYPE_OF_OBJECT"] == "OBJ") {
            std::string subPath = getExePath() + "/resources/objects/" + (item["OBJECT_SOURCE"].dump() + ".pobj");
        }
        Object obj;
        obj.NAME = item["NAME"];
        obj.TYPE_OF_OBJECT = item["TYPE_OF_OBJECT"];
        obj.X = item["X"];
        obj.Y = item["Y"];
        obj.Z = item["Z"];
        obj.W = item["W"];
        obj.H = item["H"];
        obj.D = item["D"];
        obj.R = item["R"];
        obj.G = item["G"];
        obj.B = item["B"];
        obj.PATH_TO_TEXTURE = item["PATH_TO_TEXTURE"];
        obj.MIPMAP_LEVELX = item["MIPMAP_LEVELX"];
        obj.MIPMAP_LEVELY = item["MIPMAP_LEVELY"];
        obj.MIPMAP_LEVELZ = item["MIPMAP_LEVELZ"];
        obj.SHINYNESS = item["SHINYNESS"];
        obj.ROTATION_X = item["ROTATION_X"];
        obj.ROTATION_Y = item["ROTATION_Y"];
        obj.ROTATION_Z = item["ROTATION_Z"];
        
        objects.push_back(obj);
    }
    
    filepath = getExePath() + "/resources/textures/";
    
    if (debug == true) {
        filepath = "/Users/vibingcatt/Documents/GitHub/ProtonEngine/Proton Engine/src/resources/textures/";
    }
    
    for(Object objnum : objects) {
        if(objnum.TYPE_OF_OBJECT == "CUBE") {
            new Cube(objnum.NAME,
                     glm::vec3(objnum.X, objnum.Y, objnum.Z),
                     glm::vec3(objnum.W, objnum.H, objnum.D),
                     glm::vec3(objnum.R, objnum.G, objnum.B),
                     filepath + objnum.PATH_TO_TEXTURE,
                     glm::vec3(objnum.MIPMAP_LEVELX, objnum.MIPMAP_LEVELY, objnum.MIPMAP_LEVELZ),
                     objnum.SHINYNESS, objnum.ROTATION_X, objnum.ROTATION_Y, objnum.ROTATION_Z);
        }
        if(objnum.TYPE_OF_OBJECT == "SLOPE") {
            new Slope(objnum.NAME,
                     glm::vec3(objnum.X, objnum.Y, objnum.Z),
                     glm::vec3(objnum.W, objnum.H, objnum.D),
                     glm::vec3(objnum.R, objnum.G, objnum.B),
                     filepath + objnum.PATH_TO_TEXTURE,
                     glm::vec3(objnum.MIPMAP_LEVELX, objnum.MIPMAP_LEVELY, objnum.MIPMAP_LEVELZ),
                     objnum.SHINYNESS, objnum.ROTATION_X, objnum.ROTATION_Y, objnum.ROTATION_Z);
        }
    };
}
