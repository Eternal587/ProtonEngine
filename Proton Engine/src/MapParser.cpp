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

using json = nlohmann::ordered_json;

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

std::string removeAll(std::string str, const std::string& sub_to_remove) {
    size_t pos = std::string::npos;
    while ((pos = str.find(sub_to_remove)) != std::string::npos) {
        str.erase(pos, sub_to_remove.length());
    }
    return str;
}

const bool debug = false;

void parse_map(std::string map) {
    std::vector<Cube*> Cubes;
    std::string filepath = getExePath();
    filepath = filepath.erase(filepath.length() - 14, 14) + "/resources/maps/" + map;
    
    if (debug == true) {
        filepath = "/Users/vibingcatt/Documents/GitHub/ProtonEngine/Proton Engine/src/resources/maps/" + map;
    }
    
    /// Specifies the File your accessing
    std::ifstream stream(filepath);
    
    if (!stream.is_open()) {
            std::cerr << "[ERROR] Map file not found: " << filepath << std::endl;
            return; 
    }
    
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
    
    filepath = getExePath();
    filepath = filepath.erase(filepath.length() - 14, 14) + "/resources/textures/";
    
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
        if(objnum.TYPE_OF_OBJECT == "LIGHT") {
            new LightSource(objnum.NAME, glm::vec3(objnum.X, objnum.Y, objnum.Z), glm::vec3(objnum.R, objnum.G, objnum.B));
        }
    };
}

void save_map(std::string loc) {
    
    std::string filepath = getExePath();
    filepath = filepath.erase(filepath.length() - 14, 14) + "/resources/textures/";
    
    if (debug == true) {
        filepath = "/Users/vibingcatt/Documents/GitHub/ProtonEngine/Proton Engine/src/resources/textures/";
    }
    
    json objects = json::array();
    json data;
    std::vector<Cube*> cubes = Renderer::returnCubes();
    for(Cube* cube : cubes) {
        data["NAME"] = cube->name;
        data["TYPE_OF_OBJECT"] = "CUBE";
        data["X"] = cube->position.x;
        data["Y"] = cube->position.y;
        data["Z"] = cube->position.z;
        data["W"] = cube->dimensions.x;
        data["H"] = cube->dimensions.y;
        data["D"] = cube->dimensions.z;
        data["R"] = cube->color.x;
        data["G"] = cube->color.y;
        data["B"] = cube->color.z;
        data["PATH_TO_TEXTURE"] = removeAll(cube->pathtotexture, filepath);
        data["MIPMAP_LEVELX"] = cube->tiles.x;
        data["MIPMAP_LEVELY"] = cube->tiles.y;
        data["MIPMAP_LEVELZ"] = cube->tiles.z;
        data["SHINYNESS"] = cube->shinyness;
        data["ROTATION_X"] = cube->degree_x;
        data["ROTATION_Y"] = cube->degree_y;
        data["ROTATION_Z"] = cube->degree_z;
        
        objects.push_back(data);
    }
    
    std::vector<Slope*> slopes = Renderer::returnSlopes();
    for(Slope* slope : slopes) {
        data["NAME"] = slope->name;
        data["TYPE_OF_OBJECT"] = "SLOPE";
        data["X"] = slope->position.x;
        data["Y"] = slope->position.y;
        data["Z"] = slope->position.z;
        data["W"] = slope->dimensions.x;
        data["H"] = slope->dimensions.y;
        data["D"] = slope->dimensions.z;
        data["R"] = slope->color.x;
        data["G"] = slope->color.y;
        data["B"] = slope->color.z;
        data["PATH_TO_TEXTURE"] = removeAll(slope->pathtotexture, filepath);
        data["MIPMAP_LEVELX"] = slope->tiles.x;
        data["MIPMAP_LEVELY"] = slope->tiles.y;
        data["MIPMAP_LEVELZ"] = slope->tiles.z;
        data["SHINYNESS"] = slope->shinyness;
        data["ROTATION_X"] = slope->degree_x;
        data["ROTATION_Y"] = slope->degree_y;
        data["ROTATION_Z"] = slope->degree_z;
        
        objects.push_back(data);
    }
    std::vector<LightSource*> lights = Renderer::returnLights();
    for (LightSource* light : lights) {
        data["NAME"] = light->name;
        data["TYPE_OF_OBJECT"] = "LIGHT";
        data["X"] = light->position.x;
        data["Y"] = light->position.y;
        data["Z"] = light->position.z;
        data["W"] = 0.0f;
        data["H"] = 0.0f;
        data["D"] = 0.0f;
        data["R"] = light->color.x;
        data["G"] = light->color.y;
        data["B"] = light->color.z;
        data["PATH_TO_TEXTURE"] = "";
        data["MIPMAP_LEVELX"] = 0.0f;
        data["MIPMAP_LEVELY"] = 0.0f;
        data["MIPMAP_LEVELZ"] = 0.0f;
        data["SHINYNESS"] = 0.0f;
        data["ROTATION_X"] = 0.0f;
        data["ROTATION_Y"] = 0.0f;
        data["ROTATION_Z"] = 0.0f;
    }
    
    filepath = getExePath();
    filepath = filepath.erase(filepath.length() - 14, 14) + "/resources/maps/";
    
    if (debug == true) {
        filepath = "/Users/vibingcatt/Documents/GitHub/ProtonEngine/Proton Engine/src/resources/maps/";
    }
    
    std::ofstream output;
    std::string file = filepath.append(loc.append(".pmap"));
    
    output.open(file);
    if(output.is_open()) {
        output << objects.dump();
        std::cout << "Map Successfully Saved!";
    } else {
        std::cout << "File Failed to Save";
    }
    
    
}

void unloadMap() {
    std::vector<Cube*> cubes = Renderer::returnCubes();
    std::vector<Slope*> slopes = Renderer::returnSlopes();
    std::vector<LightSource*> lights = Renderer::returnLights();
    int cubesDeleted = 0;
    int slopesDeleted = 0;
    int lightsDeleted = 0;
    
    for(Cube* cube : cubes) {
        cube->toDelete = true;
        cubesDeleted++;
    }
    for(Slope* slope : slopes) {
        slope->toDelete = true;
        slopesDeleted++;
    }
    for(LightSource* light : lights) {
        light->toDelete = true;
        lightsDeleted++;
    }
    
    std::cout << "Deleted " << cubesDeleted << " cubes and " << slopesDeleted << " slopes!";
}
