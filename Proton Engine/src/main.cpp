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
/// Macos Library for getting exe location
#include <mach-o/dyld.h>

/// Windows Specific Lib
// #include <windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

/// Proton Engine Specific Headers
#include "shapes.h"
#include "shapes_renderer.h"

<<<<<<< Updated upstream
<<<<<<< Updated upstream
/// Yaw - Side to Side Angle, Pitch - Up and Down Angle
=======
int cameramovement = 1;

>>>>>>> Stashed changes
=======
int cameramovement = 1;

>>>>>>> Stashed changes
float yaw = -90.0f; // start facing forward (−Z)
float pitch = 0.0f;

/// Tracking the Last Mouse Pos
static float lastX = 320.0f;
static float lastY = 240.0f;

/// Checking if its the first Mouse Movement
static bool firstMouse = true;

/// Mouse Callback for GLFW
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{    
    /// The Sensitivty of the Mouse - TODO: ADD ADJUSTABLE SETTING FOR SENSITIVITY
    static float sensitivity = 0.2f;
<<<<<<< Updated upstream
<<<<<<< Updated upstream

    /// IF its the first mouse movement set the lastX and lastY to the current pos of the mouse and set firstMouse to false
    if (firstMouse) {
=======
=======
>>>>>>> Stashed changes
    
    if(cameramovement == 1) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
        lastX = xpos;
        lastY = ypos;
        
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        
        yaw   += xoffset;
        pitch += yoffset;
        
        // clamp the pitch so we don't flip upside down
        if (pitch > 89.0f)  pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }
<<<<<<< Updated upstream
<<<<<<< Updated upstream

    /// Get the Offset of the last Mouse Pos and Current Pos
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    /// Set the Last MousePos to the current Mouse Pos
    lastX = xpos;
    lastY = ypos;

    /// Multiply the Offset by the Mouse sensitivity
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    /// Add the offset to the yaw and pitch 
    yaw   += xoffset;
    pitch += yoffset;

    // clamp the pitch so we don't flip upside down
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
}




/// Mac Os Version of getExecutablePath

std::string getExecutablePath() {
    /// Buffer for the path to sit in
    char pathBuffer[1024];

    /// Get the size of the pathBuffer
    uint32_t size = sizeof(pathBuffer);

    /// If the executable path is not big enough return and error
    if (_NSGetExecutablePath(pathBuffer, &size) != 0) {
        // Buffer too small; allocate dynamically if you want
        std::cerr << "Path buffer too small\n";
        return "";
    }

    /// Return the Path
    return std::string(pathBuffer);
}

/// Windows Version Of GetExecutable
/*
std::string getExecutablePath() {
    /// Create a buffer for the path to sit in
    char buffer[MAX_PATH];
    /// Push the Path into the buffer
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    /// Set the path equal to the path in the buffer
    std::string path = std::string(buffer);
    /// Erase the name of the executable EX: (28 in this case because that is the current size of the title)
    path.erase(path.end() - 28, path.end());
    /// Return the Executable Path
    return path;
} */

/// Parsing the shader from a file

/// Creating a struct to contain the source so you can have on function to return both sources
struct shader_program_source {
    std::string vertex_source;
    std::string fragment_source;
};

static shader_program_source parse_shader(const std::string& filepath) {
    /// Specifies the File your accessing
    std::ifstream stream(filepath);

    /// Creating a Variable for the current line to go into
    std::string line;

    /// If the stream isn't open the file can't be opened so return nothing and put a error into the console
    if (!stream.is_open()) {
            std::cerr << "parse_shader: Failed to open file: " << filepath << std::endl;
            return {"", ""};
    }

    /// Create a Class for the shader type to make easy transition to the array the data is being put into
    enum class shader_type {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    
    /// creating a buffer for the shader code to be in and set the shader type to none
    std::stringstream ss[2];
    shader_type type = shader_type::NONE;
    
    /// while getline returns an actual line check if the code can find #shader then check if that line has vertex or fragment and change depending on it
    while(getline(stream, line)) {
        /// If the line can find shader then set the current type to vertex or fragment depending on where it needs to start dumping to
        if(line.find("#shader") != std::string::npos) {
            if(line.find("vertex") != std::string::npos) {
                type = shader_type::VERTEX;
            } else if(line.find("fragment") != std::string::npos)  {
                type = shader_type::FRAGMENT;
            }
        } else {
            /// If the line does not find shader then dump it to its respective shader
            ss[(int)type] << line << '\n';
        }
    }
    /// Return the shaders
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
    
    /// Attaching the shaders to the program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    /// Linking the program to Proton Engine and validating it
    glLinkProgram(program);
    glValidateProgram(program);

    /// Delete the shaders as they are no longer usefuln and return the shader program
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

/// Fallback Shader (not currently setup)
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

    /// Create the GLFW Window and if when asked to return the window object it returns nothing return an error into the console
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

    /// Tell OpenGL the Viewport to the window
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    
    /// VBO: Video Buffer Object, The VBO is GPU Memory used for storing your vertex data, instead of sending vertex data from your cpu to your gpu every frame you upload it once through the VBO and the GPU can then access the data every time you draw
    /// VAO: Vertex Attribute Object. used for refrencing your Vertex Atributes, telling your GPU how to read your data in the VBO, telling it what index your data is at
    /// IBO: Index Buffer Object, used to tell OpenGL to go over a vertex again to save space by not having to repeat verticies
    
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

    /// Get the file path to the executable
    std::string filepath = getExecutablePath();
    
    std::cout << "Executable Being Run at: " << filepath << std::endl;
    
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    bool debug = false;

    /// If I'm Debugging in xcode it runs the executable in a very specific spot that does not contain the resources folder, meaning I wont be able to access my textures, models, and shaders
=======
=======
>>>>>>> Stashed changes
    bool debug = true;
    
>>>>>>> Stashed changes
    if (debug == true) {
        filepath = "/Users/vibingcatt/Documents/GitHub/ProtonEngine/Proton Engine/src";
    }
    
    // shader_program_source source = parse_shader(filepath + "resources\\shaders\\basic.glsl"); // Windows Version
    shader_program_source source = parse_shader(filepath + "/resources/shaders/basic.glsl"); // Mac Version
    unsigned int shader = create_shaders(source.vertex_source, source.fragment_source);

    /// Tell Open GL to use the shader program
    glUseProgram(shader);

    /// Setting up all the cubes (manually as of current but will be adding maps to easily add objects)

    /// Windows Textures Locations
    /*
    Cube cube1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "resources\\textures\\cat.jpg", glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
    Cube cube2(glm::vec3(9.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "resources\\textures\\cat.jpg", glm::vec3(1.0f, 1.0f, 1.0f), 0.2f);
    
    Cube crate(glm::vec3(4.75f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "resources\\textures\\crate.png", glm::vec3(1.0f, 1.0f, 1.0f), 0.2f);
    
    Cube platform(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "resources\\textures\\prototype.png", glm::vec3(10.0f, 1.0f, 10.0f), 1.0f);
    
    Cube wall1(glm::vec3(0.0f, 0.0f, -0.5f), glm::vec3(10.0f, 1.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "resources\\textures\\stone_bricks.png", glm::vec3(30.0f, 6.0f, 2.0f), 0.1f);
    Cube wall2(glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.5f, 1.5f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "resources\\textures\\stone_bricks.png", glm::vec3(2.0f, 6.0f, 30.0f), 0.1f);
    Cube wall3(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(10.0f, 1.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "resources\\textures\\stone_bricks.png", glm::vec3(30.0f, 6.0f, 2.0f), 0.1f);
    Cube wall4(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.5f, 1.5f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "resources\\textures\\stone_bricks.png", glm::vec3(2.0f, 6.0f, 30.0f), 0.1f);
    */

    /// MacOS Texture Locations
    Cube cube1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "/resources/textures/cat.jpg", glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
    Cube cube2(glm::vec3(9.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "/resources/textures/cat.jpg", glm::vec3(1.0f, 1.0f, 1.0f), 0.2f);
    
    Cube crate(glm::vec3(4.75f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "/resources/textures/crate.png", glm::vec3(1.0f, 1.0f, 1.0f), 0.2f);
    
    Cube platform(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "/resources/textures/prototype.png", glm::vec3(10.0f, 1.0f, 10.0f), 1.0f);
    
    Cube wall1(glm::vec3(0.0f, 0.0f, -0.5f), glm::vec3(10.0f, 1.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "/resources/textures/stone_bricks.png", glm::vec3(30.0f, 6.0f, 2.0f), 0.1f);
    Cube wall2(glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.5f, 1.5f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "/resources/textures/stone_bricks.png", glm::vec3(2.0f, 6.0f, 30.0f), 0.1f);
    Cube wall3(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(10.0f, 1.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "/resources/textures/stone_bricks.png", glm::vec3(30.0f, 6.0f, 2.0f), 0.1f);
    Cube wall4(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.5f, 1.5f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), filepath + "/resources/textures/stone_bricks.png", glm::vec3(2.0f, 6.0f, 30.0f), 0.1f);
    
    
    

    /// Struct for the camera object that contains the yaw (side to side), the pitch (up and down) and the position
    struct Camera {
        glm::vec3 position;
        float yaw;   // rotation around Y axis (like your cangle)
        float pitch; // rotation around X axis (for looking up/down)
    };

    /// Creating a camera for the player
    Camera player;

    /// Defining the postiion, yaw, and pitch of the player
    player.position = glm::vec3(5.0f, 1.0f, 5.0f);
    player.yaw = 0.0f;
    player.pitch = 0.0f;

    /// Calculation where the player is facing, finding their normal vector
    glm::vec3 front;
    front.x = cos(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
    front.y = sin(glm::radians(player.pitch));
    front.z = sin(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
    glm::vec3 camera_front = glm::normalize(front);

    // Create transformation matrices
    glm::mat4 model = glm::mat4(1.0f);

    /// Creating the transformation matrix thats used to change by where the camera is looking
    glm::mat4 view = glm::lookAt(
        player.position,  // Camera position
        player.position + camera_front,  // Target
        glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
    );

    /// Creating the transformation matrix for multiplying by the aspect ratio, fov, and zoom
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        16.0f / 9.0f, /// Aspect Ratio
        0.9f, /// Zoom
        200.0f /// FOV
    );

    // Send all 3 matrices to shader
    int modelLoc = glGetUniformLocation(shader, "model");
    int viewLoc  = glGetUniformLocation(shader, "view");
    int projLoc  = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(projection));

    /// Defining the speed of the player on the x, y, and z axis's
    float speed = 0.02f;

    /// Enabling the check for depth so that OpenGL Dosen't Render Object that are behind other Objects
    glEnable(GL_DEPTH_TEST);

    /// Setting an Input mode up for the cursor and disabling the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /// Setting up the callback for the cursor pos
    glfwSetCursorPosCallback(window, mouse_callback);

    /// Setting up full screen by getting the monitor and mode then telling it to use the current resolution of the screen and refresh rate
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    glfwSetWindowMonitor(window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
<<<<<<< Updated upstream
<<<<<<< Updated upstream

    /// Main Game Loop
    while (!glfwWindowShouldClose(window))
    {
        
        // Checking for the Quit Button,resize button, ect
=======
    
    
    
    
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    static int cubeCounter = 0;
    

    
    while (!glfwWindowShouldClose(window))
    {
=======
    
    
    
    
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    static int cubeCounter = 0;
    

    
    while (!glfwWindowShouldClose(window))
    {
>>>>>>> Stashed changes
        // input + timing
>>>>>>> Stashed changes
        glfwPollEvents();

        // Update camera front vector from yaw & pitch 
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera_front = glm::normalize(front);

        /// Get the Current Time
        float pt = t;
        float t = glfwGetTime();
<<<<<<< Updated upstream
<<<<<<< Updated upstream

        float deltatime = t - pt;
        
=======
>>>>>>> Stashed changes

        /// Checking if a key is pressed -> moving the camera because of it
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player.position += speed * camera_front; /// Moving the camera forward based on the Forward Vector
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player.position -= speed * camera_front; /// Moving the camera backwards based on he forward vector
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) player.position.y += speed; /// Modifiying the y to go up
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) player.position.y -= speed; /// Modifing the y to go down
        glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, glm::vec3(0.0f, 1.0f, 0.0f))); /// Getting the vector of the right of the playe ror "camera_right"
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player.position -= speed * camera_right; /// Going to the left based on the right vector 
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player.position += speed * camera_right; /// Going to the right based on the right vectir

        /// Preparing the Matirces
=======

        /// Checking if a key is pressed -> moving the camera because of it
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player.position += speed * camera_front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player.position -= speed * camera_front;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) player.position.y += speed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) player.position.y -= speed;
        glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, glm::vec3(0.0f, 1.0f, 0.0f)));
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player.position -= speed * camera_right;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player.position += speed * camera_right;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cameramovement ^= 1;
        
        if (cameramovement == 1) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else if (cameramovement == 0) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        // Prepare matrices (recompute each frame)
>>>>>>> Stashed changes
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view = glm::lookAt(player.position, player.position + camera_front, glm::vec3(0.0f, 1.0f, 0.0f));

        /// Getting the frame buffer for the width and height
        int fbw, fbh;
        glfwGetFramebufferSize(window, &fbw, &fbh);
        /// Getting the aspect ratio based on the width and height
        float aspect = (fbh > 0) ? (float)fbw / (float)fbh : 4.0f/3.0f;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Object Manager");
        
        const auto& cubes = Renderer::GetCubes();
        for (size_t i = 0; i < cubes.size(); ++i) {
            Cube* cube = cubes[i];

            // Display some info about the cube
            ImGui::Text("Cube %zu:", i);
            
            ImGui::SliderFloat3(("Cube " + std::to_string(i) + " Position").c_str(), &cube->position.x, -10.0f, 10.0f);
            ImGui::ColorEdit3(("Cube " + std::to_string(i) + " Color").c_str(), &cube->color.r);

            ImGui::Separator();
        }

        if (ImGui::Button("Add Cube")) {
            // Increment counter so each cube has a different position
            cubeCounter++;

            // Example: position new cubes in a line along X
            glm::vec3 newPos(cubeCounter * 2.0f, 0.0f, 0.0f);
            glm::vec3 dimensions(1.0f, 1.0f, 1.0f);
            glm::vec3 color(1.0f, 0.0f, 0.0f); // red cube
            std::string texturePath = filepath + "/resources/textures/cat.jpg"; // your texture path
            glm::vec3 tiles(1.0f, 1.0f, 1.0f);
            float shininess = 0.5f;

            // Create a new cube on the heap
            Cube* newCube = new Cube(newPos, dimensions, color, texturePath, tiles, shininess);

            // Renderer::RegisterCube is called automatically in Cube constructor
        }

        ImGui::End(); // Always call End() for every Begin()

        
        
        
        
        
        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader);
        Renderer::RenderAll(shader);
        
        // Render everything
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // upload uniforms AFTER glUseProgram
        int modelLoc = glGetUniformLocation(shader, "model");
        int viewLoc  = glGetUniformLocation(shader, "view");
        int projLoc  = glGetUniformLocation(shader, "projection");
        
        int lightLoc = glGetUniformLocation(shader, "alight_sources");
        int lightColLoc = glGetUniformLocation(shader, "alight_color");
        int CamPosLoc = glGetUniformLocation(shader, "viewPos");
        
        glUniform3f(CamPosLoc, player.position.x, player.position.y, player.position.z);
        
        /// Uploading the Light Postion and Color
        glUniform3f(lightLoc, 5.0f, 5.0f, 5.0f);
        glUniform3f(lightColLoc, 1.0f, 1.0f, 1.0f);
        
        if (modelLoc >= 0) glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        if (viewLoc  >= 0) glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
        if (projLoc  >= 0) glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(projection));

        /// Updating the viewport every frame
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        /// Swaping the buffers
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

}
