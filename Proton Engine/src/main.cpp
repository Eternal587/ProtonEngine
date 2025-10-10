//
//  main.cpp
//  Proton Engine
//
//  Created by VibingCatt on 10/5/25.
//

#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include <mach-o/dyld.h>

std::string getExecutablePath() {
    char pathBuffer[1024];
    uint32_t size = sizeof(pathBuffer);
    if (_NSGetExecutablePath(pathBuffer, &size) != 0) {
        // Buffer too small; allocate dynamically if you want
        std::cerr << "Path buffer too small\n";
        return "";
    }
    return std::string(pathBuffer);
}


/// Parsing the shader from a file

struct shader_program_source {
    std::string vertex_source;
    std::string fragment_source;
};

static shader_program_source parse_shader(const std::string& filepath) {
    /// Specifies the File your accessing
    std::ifstream stream(filepath);
    
    std::string line;
    
    if (!stream.is_open()) {
            std::cerr << "parse_shader: Failed to open file: " << filepath << std::endl;
            return {"", ""};
    }
    
    enum class shader_type {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    
    /// creating a buffer for the shader code to be in
    std::stringstream ss[2];
    shader_type type = shader_type::NONE;
    
    /// while getline returns an actual line check if the code can find #shader then check if that line has vertex or fragment and change depending on it
    while(getline(stream, line)) {
        if(line.find("#shader") != std::string::npos) {
            if(line.find("vertex") != std::string::npos) {
                type = shader_type::VERTEX;
            } else if(line.find("fragment") != std::string::npos)  {
                type = shader_type::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << '\n';
        }
    }
    
    return {ss[0].str(), ss[1].str()};
}

/// Shaders, used for rendering using OpenGL's Graphics Pipeline

static unsigned int compile_shader(const std::string& source, unsigned int type) {
    /// Defining the Vertex shader glCreateShader(shadertype)
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    
    /// glShaderSource(shader, count of source codes, pointer to source, length)
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    /// Error Handeling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex":"fragment") << " shader:\n" << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

static unsigned int create_shaders(const std::string& vertex_shader, const std::string& fragment_shader) {
    /// Creating the program for the shader
    unsigned int program = glCreateProgram();
    
    /// Compiling the Shaders
    unsigned int vs = compile_shader(vertex_shader, GL_VERTEX_SHADER);
    unsigned int fs = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);
    
    /// At taching the shaders to the program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    /// Linking the program to Proton Engine and validating it
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

const char* fallback_vertex = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0, 1.0);
}
)";

const char* fallback_fragment = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.2, 0.7, 0.3, 1.0);
}
)";



/// Error Callback for GLFW ->  Used to report error with GLFW

void error_callback(int error, const char* description)
{
    std::cout << "Error: \n" << description << std::endl;
}

/// Key Callback for GLFW -> Used to detect key presses and do certain things

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main()
{
    /// Check to make sure GLFW Was Initilzed Properly
    
    if(!glfwInit()) {
        std::cout << "GLFW Failed to Initalize" << std::endl;
    }
    
    /// Used to tell where GLFW to dump any errors it has
    
    glfwSetErrorCallback(error_callback);
    
    /// Creating the window object
    
    /// Window "hints" are used to tell GLFW important things before creation of a window in this case the version and OpenGL Profile
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "Proton Engine v0.0.1", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW failed to create a window\n";
    }
    
    /// Telling the OpenGL API what the window context is
    glfwMakeContextCurrent(window);
    
    /// Telling glad to Load OpenGL
    gladLoadGL();
    glfwSwapInterval(1);
    
    /// Telling GLFW what to call back to when interperting inputs
    glfwSetKeyCallback(window, key_callback);
    
    /// Paramters for OpenGL To display into the window with
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    float positions[12] = {-0.5f, -0.5f, /// 0
                            0.5f, -0.5f, /// 1
                            0.5f, 0.5f, /// 2
                            -0.5f, 0.5f}; /// 3
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    /// VBO: Video Buffer Object, The VBO is GPU Memory used for storing your vertex data, instead of sending vertex data from your cpu to your gpu every frame you upload it once through the VBO and the GPU can then access the data every time you draw
    /// VAO: Vertex Attribute Object. used for refrencing your Vertex Atributes, telling your GPU how to read your data in the VBO, telling it what index your data is at
    /// IBO: Index Buffer Object, used to tell OpenGL to go over a vertex again to save space by not having to repeat verticies
    
    unsigned int VBO, VAO, IBO;
    
    /// Generating buffer (num_of_buffers, buffer var)
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    
    /// Bind the Buffer (Type of data being stored, buffer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    
    /// Put data in the buffer (type of data, size in bytes, data, how your using the data)
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), positions, GL_STATIC_DRAW);
    
    /// Vertex Attrib Pointer (just telling OpenGL how to deal with vertex data) glVertexAttribPointer(index of attribute, number of components per vertex attribute, data type (float, int, ect), normalized(setting the attribute to a "normal" value (between 0-1), amount of bytes between each vertex, offset of each attribute in bytes)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    
    /// Enabling a Vertex Attribute
    glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    
    
    /// Shaders: Pipeline
    ///
    /// Draw Call
    /// ->
    /// Vertex Shader:
    /// Called for each vertex, telling where the vertex to be on the screen (also used to pass data to the next shader)
    /// ->
    /// Fragment/Pixel Shader:
    /// Run for each Pixel that needs to be drawn, Resterizing Each Pixel, Decides which color each pixel needs to be
    /// ->
    /// Drawn on Screen
    
    std::string filepath = getExecutablePath();
    
    bool debug = true;
    
    if (debug == true) {
        filepath = "/Users/vibingcatt/Documents/GitHub/ProtonEngine/Proton Engine/src";
    }
    
    shader_program_source source = parse_shader(filepath + "/resources/shaders/basic.glsl");
    unsigned int shader = create_shaders(source.vertex_source, source.fragment_source);
    
    /// Main Game Loop
    
    while (!glfwWindowShouldClose(window))
    {
        /// Timer, used for animations, ect
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        glBindVertexArray(VAO);
        
        double time = glfwGetTime();
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        
        /// Swapping the "front and back" video buffers GLFW Uses
        
        glfwSwapBuffers(window);
        
        /// Handles Window Inputs connecting GLFW with the window system
        
        glfwPollEvents();
    }

    /// Destroying the GLFW Window
    glfwDestroyWindow(window);
    
    /// Terminating GLFW
    glfwTerminate();
}
