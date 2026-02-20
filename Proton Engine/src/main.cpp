//
//  main.cpp
//  Proton Engine
//
//  Created by VibingCatt on 10/5/25.
//

#include <iostream>

/// IMGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

/// Glad and GLFW
#include "glad.h"
#include <GLFW/glfw3.h>

/// Strings and Text
#include <fstream>
#include <string>
#include <sstream>

/// Macos Library for getting exe location
#include <mach-o/dyld.h>

/// Windows Specific Lib
// #include <windows.h>

/// GLM & Other Math
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <cmath>
#include <chrono>

/// Audio
#include <OpenAL/OpenAL.h>

/// Proton Engine Specific Headers
#include "shapes.h"
#include "shapes_renderer.h"
#include "MapParser.h"
#include "Models.h"

float yaw = -90.0f; // start facing forward (−Z)
float pitch = 0.0f;

static float lastX = 320.0f; // center of window (half width)
static float lastY = 240.0f; // center of window (half height)
static bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static float sensitivity = 0.2f;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
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

/// There is going to be 2 types of hitbox to make some collisions simplier for speed reasons
///
/// First type hit "box"
/// Its going to be a Cube
///
/// Second type
/// Hitbox Mesh
///
/// Its a mesh as a hitbox

struct Collision {
public:
    bool col;
    bool top;
    Collision(bool coll, bool topp) : col(coll), top(topp){}
};

Collision HitboxIntersect(Hitbox hitbox1, Hitbox hitbox2) { // HitboxIntersect(HITBOX1, HITBOX2) -> Tests if 2 Hitboxes are colliding

    glm::vec3 hitboxPoints[] = {
        hitbox1.position,
        hitbox1.position + glm::vec3(hitbox1.dimensions.x, 0, 0),
        /// +Z
        hitbox1.position + glm::vec3(0, 0, hitbox1.dimensions.z),
        hitbox1.position + glm::vec3(hitbox1.dimensions.x, 0, hitbox1.dimensions.z),
        hitbox1.position + glm::vec3(0, hitbox1.dimensions.y, 0), /// Top Face
        hitbox1.position + glm::vec3(hitbox1.dimensions.x, hitbox1.dimensions.y, 0), /// Top Face
        hitbox1.position + glm::vec3(0, hitbox1.dimensions.y, hitbox1.dimensions.z), /// Top Face
        hitbox1.position + glm::vec3(hitbox1.dimensions.x, hitbox1.dimensions.y, hitbox1.dimensions.z)}; /// Top Face
    for(int i = 0; i < 4; i++) {
        /// X AXIS CHECK
        if(hitboxPoints[i].x > hitbox2.position.x && hitboxPoints[i].x < hitbox2.position.x + hitbox2.dimensions.x) {
            /// Y AXIS CHECK
            if(hitboxPoints[i].y > hitbox2.position.y && hitboxPoints[i].y < hitbox2.position.y + hitbox2.dimensions.y) {
                /// Z AXIS CHECK
                if(hitboxPoints[i].z > hitbox2.position.z && hitboxPoints[i].z < hitbox2.position.z + hitbox2.dimensions.z) {
                    /// COLLISON!
                    return Collision(true, true);
                }
            }
        }
    }
    
    /// Checking for the  bottom face collision ^^^ (im planning on using it later for gravity)
    
    for(int i = 3; i < 8; i++) {
        /// X AXIS CHECK
        if(hitboxPoints[i].x > hitbox2.position.x && hitboxPoints[i].x < hitbox2.position.x + hitbox2.dimensions.x) {
            /// Y AXIS CHECK
            if(hitboxPoints[i].y > hitbox2.position.y && hitboxPoints[i].y < hitbox2.position.y + hitbox2.dimensions.y) {
                /// Z AXIS CHECK
                if(hitboxPoints[i].z > hitbox2.position.z && hitboxPoints[i].z < hitbox2.position.z + hitbox2.dimensions.z) {
                    /// COLLISON!
                    return Collision(true, false);
                }
            }
        }
    }
    // ELSE return this false
    return Collision(false, false);
}


/// Mac Os Version

std::string getExecutablePath() { /// Returns the Exectuable Path
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
std::string getExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string path = std::string(buffer);
    path.erase(path.end() - 28, path.end());
    return path;
} */

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
    
    std::string filepath = getExecutablePath();
    
    std::cout << "Executable Being Run at: " << filepath << std::endl;
    
    bool debug = true;
    
    if (debug == true) {
        filepath = "/Users/vibingcatt/Documents/GitHub/ProtonEngine/Proton Engine/src";
    }
    
    // shader_program_source source = parse_shader(filepath + "resources\\shaders\\basic.glsl");
    shader_program_source source = parse_shader(filepath + "/resources/shaders/basic.glsl"); // Mac Version
    unsigned int shader = create_shaders(source.vertex_source, source.fragment_source);
    
    glUseProgram(shader);
    
    parse_map("Basic.amap");
    
    struct Camera {
        glm::vec3 position;
        glm::vec3 last_position;
        float yaw;   // rotation around Y axis (like your cangle)
        float pitch; // rotation around X axis (for looking up/down)
        Hitbox hitbox;
        float yvelocity; // Used for gravity
    };
    
    Camera player;
    
    player.position = glm::vec3(6.0f, 3.0f, 5.0f);
    player.yaw = 0.0f;
    player.pitch = 0.0f;
    player.hitbox.position = glm::vec3(player.position.x, (player.position.y - 0.6f), player.position.z);
    player.hitbox.dimensions = glm::vec3(0.1f, 0.7f, 0.1f);
    player.hitbox.dir = glm::vec3(player.pitch, player.yaw, 0.0f);
    player.last_position = player.position;
    player.yvelocity = 0;
    
    glm::mat4 rot =
    glm::rotate(glm::mat4(1.0f), glm::radians(player.hitbox.dir.x), glm::vec3(1,0,0))
    * glm::rotate(glm::mat4(1.0f), glm::radians(player.hitbox.dir.y), glm::vec3(0,1,0))
    * glm::rotate(glm::mat4(1.0f), glm::radians(player.hitbox.dir.z), glm::vec3(0,0,1));
    
    player.hitbox.axes[0] = glm::normalize(glm::vec3(rot * glm::vec4(1,0,0,0)));
    player.hitbox.axes[1] = glm::normalize(glm::vec3(rot * glm::vec4(0,1,0,0)));
    player.hitbox.axes[2] = glm::normalize(glm::vec3(rot * glm::vec4(0,0,1,0)));
    
    glm::vec3 front;
    front.x = cos(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
    front.y = sin(glm::radians(player.pitch));
    front.z = sin(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
    glm::vec3 camera_front = glm::normalize(front);
    
    // Create transformation matrices
    glm::mat4 model = glm::mat4(1.0f);
    
    glm::mat4 view = glm::lookAt(
                                 player.position,  // Camera position
                                 player.position + camera_front,  // Target
                                 glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
                                 );
    
    glm::mat4 projection = glm::perspective(
                                            glm::radians(45.0f),
                                            16.0f / 9.0f,
                                            0.9f,
                                            200.0f
                                            );
    
    // Send all 3 matrices to shader
    int modelLoc = glGetUniformLocation(shader, "model");
    int viewLoc  = glGetUniformLocation(shader, "view");
    int projLoc  = glGetUniformLocation(shader, "projection");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(projection));
    
    float speed = 0.02f;
    
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    
    int mouse = 0;
    
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    
    glfwSetWindowMonitor(window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    
    /// Enabling Blending for use of transparent objects
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    float previous_time = 0.0f;
    int framecount = 0;


    /// ImGui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    
    
    bool touching = false;
    
    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    float gravity = -0.05f;
    
    while (!glfwWindowShouldClose(window))
    {
        // input + timing
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::ShowStyleEditor();
        
        // Update camera front vector from yaw & pitch (recompute each frame)
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera_front = glm::normalize(front);
        
        std::vector<Hitbox> hitboxes = returnHitboxes();
        
        float currentTime = deltaTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        
        player.last_position = player.position;
        
        
        
        /// Checking if a key is pressed -> moving the camera because of it
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player.position += speed * glm::vec3(camera_front.x, 0.0f, camera_front.z);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player.position -= speed * camera_front;
        /*if (touching == true) {
         if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) player.yvelocity += 1.0 ;
         }
         if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) player.yvelocity -= 0.1;*/
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) player.position.y += speed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) player.position.y -= speed;
        glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, glm::vec3(0.0f, 1.0f, 0.0f)));
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player.position -= speed * camera_right;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player.position += speed * camera_right;
        
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) player.yvelocity += 0.1 ;
        
        if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
            if (mouse == 0) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                mouse = 1;
            } else if (mouse == 1) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                mouse = 0;
            }
        }
        
        
        player.yaw = yaw;
        player.pitch = pitch;
        
        player.hitbox.position = glm::vec3(player.position.x - (0.5 * player.hitbox.dimensions.x), player.position.y - (player.hitbox.dimensions.y), player.position.z - (0.5 * player.hitbox.dimensions.z));
        player.hitbox.dir = glm::vec3(player.pitch, 0.0f, 0.0f);
        
        
        touching = false;
        
        player.position.y += player.yvelocity;
        /*
         for(int i = 0; i < hitboxes.size(); i++) {
         if(HitboxIntersect(player.hitbox, hitboxes[i]).top) {
         player.position.y = player.last_position.y;
         player.yvelocity = 0;
         touching = true;
         for(int i = 0; i < hitboxes.size(); i++) {
         if(HitboxIntersect(player.hitbox, hitboxes[i]).col) {
         player.position.x = player.last_position.x;
         player.position.z = player.last_position.z;
         touching = true;
         }
         }
         }
         }
         
         player.position.y -= player.yvelocity;
         
         if(!touching) {
         player.yvelocity += gravity * deltaTime;
         }
         
         player.position.y += player.yvelocity;
         */
        
        for(int i = 0; i < hitboxes.size(); i++) {
            if(HitboxIntersect(player.hitbox, hitboxes[i]).col) {
                player.position = player.last_position;
                touching = true;
                
            }}
            player.hitbox.position = glm::vec3(player.position.x - (0.5 * player.hitbox.dimensions.x), player.position.y - (0.5 * player.hitbox.dimensions.y), player.position.z - (0.5 * player.hitbox.dimensions.z));
            
            
            // Prepare matrices (recompute each frame)
            glm::mat4 model = glm::mat4(1.0f);
            // example rotating model over time:
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // slight tilt
            
            glm::mat4 view = glm::lookAt(player.position, player.position + camera_front, glm::vec3(0.0f, 1.0f, 0.0f));
            
            int fbw, fbh;
            glfwGetFramebufferSize(window, &fbw, &fbh);
            float aspect = (fbh > 0) ? (float)fbw / (float)fbh : 4.0f/3.0f;
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
            
            // render
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(shader);
            Renderer::RenderAll(shader);
            
            // ImGui Menus
            ImGui::Begin("FPS");
            ImGui::Text(std::to_string(io.Framerate).c_str());
            ImGui::Text(std::to_string(player.yvelocity).c_str());
            ImGui::End();

            ImGui::Begin("DEBUG MENU");
            if(ImGui::BeginTabBar("Debug")) {
                if(ImGui::BeginTabItem("Object Manager")) {
                    
                    if(ImGui::BeginTabBar("Objects")) {
                        
                        if(ImGui::BeginTabItem("Cubes")) {
                            for(int i = 0; i < cubes.size(); i++) {
                                if(ImGui::CollapsingHeader(cubes[i].name)) {
                                    // Positions
                                    If(ImGui::BeginTabBar("Attributes")) {
                                        if(ImGui::BeginTabItem("Position")) {
                                            // X Corrdinate
                                            
                                            ImGui::Text(std::to_string(cubes[i].position.x));
                                            
                                            ImGui::SameLine();
                                            
                                            if(ImGui::Button("-")){
                                                cubes[i].position.x = cubes[i].position.x - 0.1;
                                            }
        
                                            ImGui::SameLine();
        
                                            if(ImGui::Button("+")){
                                                cubes[i].position.x = cubes[i].position.x + 0.1;
                                            }
        
                                            // Y Corrdinate
        
                                            ImGui::Text(std::to_string(cubes[i].position.y));
                                            
                                            ImGui::SameLine();
                                            
                                            if(ImGui::Button("-")){
                                                cubes[i].position.y = cubes[i].position.y - 0.1;
                                            }
        
                                            ImGui::SameLine();
        
                                            if(ImGui::Button("+")){
                                                cubes[i].position.y = cubes[i].position.y + 0.1;
                                            }
        
                                            // Z Corrdinate
        
                                            ImGui::Text(std::to_string(cubes[i].position.z));
                                            
                                            ImGui::SameLine();
                                            
                                            if(ImGui::Button("-")){
                                                cubes[i].position.z = cubes[i].position.z - 0.1;
                                            }
        
                                            ImGui::SameLine();
        
                                            if(ImGui::Button("+")){
                                                cubes[i].position.z = cubes[i].position.z + 0.1;
                                            }

                                            ImGui::EndTabItem();
                                        }
    
                                        // Dimensions
                                        if(ImGui::BeginTabItem("Dimensions")) {
        
                                            // X Dimension
                                            
                                            ImGui::Text(std::to_string(cubes[i].dimensions.x));
                                            
                                            ImGui::SameLine();
                                            
                                            if(ImGui::Button("-")){
                                                cubes[i].dimensions.x = cubes[i].dimensions.x - 0.1;
                                            }
        
                                            ImGui::SameLine();
        
                                            if(ImGui::Button("+")){
                                                cubes[i].dimensions.x = cubes[i].dimensions.x + 0.1;
                                            }
        
                                            // Y Dimension
        
                                            ImGui::Text(std::to_string(cubes[i].dimensions.y));
                                            
                                            ImGui::SameLine();
                                            
                                            if(ImGui::Button("-")){
                                                cubes[i].dimensions.y = cubes[i].dimensions.y - 0.1;
                                            }
        
                                            ImGui::SameLine();
        
                                            if(ImGui::Button("+")){
                                                cubes[i].dimensions.y = cubes[i].dimensions.y + 0.1;
                                            }
        
                                            // Z Dimensions
        
                                            ImGui::Text(std::to_string(cubes[i].dimensions.z));
                                            
                                            ImGui::SameLine();
                                            
                                            if(ImGui::Button("-")){
                                                cubes[i].dimensions.z = cubes[i].dimensions.z - 0.1;
                                            }
        
                                            ImGui::SameLine();
        
                                            if(ImGui::Button("+")){
                                                cubes[i].dimensions.z = cubes[i].dimensions.z + 0.1;
                                            }

                                            ImGui::EndTabItem();
                                        }
    
                                        // Rotations
                                        if(ImGui::BeginTabItem("Rotations")) {
    
                                            // X Axis
        
                                            ImGui::Text(std::to_string(cubes[i].degree_x));
        
                                            ImGui::SameLine();
        
                                            ImGui::SliderFloat("X Axis", &cubes[i].degree_x, 0.0f, 360.0f);
        
                                            // Y Axis
        
                                            ImGui::Text(std::to_string(cubes[i].degree_y));
        
                                            ImGui::SameLine();
        
                                            ImGui::SliderFloat("Y Axis", &cubes[i].degree_y, 0.0f, 360.0f);
        
                                            // Z Axis
        
                                            ImGui::Text(std::to_string(cubes[i].degree_z));
        
                                            ImGui::SameLine();
        
                                            ImGui::SliderFloat("Z Axis", &cubes[i].degree_z, 0.0f, 360.0f);

                                            ImGui::EndTabItem();
                                        }
                                    }
                
                                }
                            }
                        }
                        
                    ImGui::EndTabBar();
                    }
                    
                ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
            
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
            
            glUniform3f(lightLoc, 5.0f, 5.0f, 5.0f);
            glUniform3f(lightColLoc, 1.0f, 1.0f, 1.0f);
            
            if (modelLoc >= 0) glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            if (viewLoc  >= 0) glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
            if (projLoc  >= 0) glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(projection));
            
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            
            glfwSwapBuffers(window);
        }
        
        /// Destroying the GLFW Window
        glfwDestroyWindow(window);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        /// Terminating GLFW
        glfwTerminate();
    }
