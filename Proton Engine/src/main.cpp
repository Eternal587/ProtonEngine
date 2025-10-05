//
//  main.cpp
//  Proton Engine
//
//  Created by VibingCatt on 10/5/25.
//

#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>

/// Shaders, used for rendering using OpenGL's Graphics Pipeline

// Vertex Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(0.2, 0.7, 0.3, 1.0); // greenish
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

    /// Window "hints" are used to tell GLFW important things before creation of a window in this case the version
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "Proton Engine v0.0.1", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW failed to create a window\n";
    }
    
    /// Telling the OpenGL API what the window context is
    glfwMakeContextCurrent(window);
    
    /// Telling glad to Load OpenGL
    gladLoadGL();
    
    /// Telling GLFW what to call back to when interperting inputs
    glfwSetKeyCallback(window, key_callback);
    
    /// Paramters for OpenGL To display into the window with
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    /// Main Game Loop
    
    while (!glfwWindowShouldClose(window))
    {
        /// Timer, used for animations, ect
        double time = glfwGetTime();
        
        /// Swapping the "front and back" video buffers GLFW Uses
        
        glfwSwapInterval(1);
        glfwSwapBuffers(window);
        
        /// Handles Window Inputs connecting GLFW with the window system
        
        glfwPollEvents();
    }

    /// Destroying the GLFW Window
    glfwDestroyWindow(window);
    
    /// Terminating GLFW
    glfwTerminate();
}
