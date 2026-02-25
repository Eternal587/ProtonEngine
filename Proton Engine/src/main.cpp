//
//  main.cpp
//  Proton Engine
//
//  Created by VibingCatt on 10/5/25.
//

#include <iostream>

/// GLFW & Vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstdlib>

/// Strings and Text
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>

/// GLM & Other Math
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
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
#include "shaderManager.h"

#if defined(_WIN32)
    #include <windows.h>
    const std::string ARCH = "WIN32";
#elif defined(__linux__)
    // Code for Linux
    const std::string ARCH = "LINUX";
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    const std::string ARCH = "APPLE";
#else
#error "Architechure Platform Unknown, Proton Engine only supports Windows, MacOS, and Linux"
#endif

const char* PROJECT_NAME = "Proton Engine v0.0.3";
const uint32_t WIDTH = 800; // Defining the starting Width for the GLFW Window
const uint32_t HEIGHT = 600; // Defining the starting Height for the GLFW Window

// Error Handling / Validation Layers
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

// Check for validation layer support
bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr); // Getting Layer Count
    
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); // Getting Layers Supported
    
    for (const char* layerName : validationLayers) {
        bool layerFound = false;
        
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;\
                break;
            }
        }
        
        if (!layerFound) {
            return false;
        }
    }
    return true;
}

// Application Class
class Application {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    VkInstance instance; // Creates the Vulkan Instance
    GLFWwindow* window; // Creates the GLFW Window Object
    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Disabling GLFW from automatically attempting to use OpenGL
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Telling GLFW to disable window resizing
        
        window = glfwCreateWindow(WIDTH, HEIGHT, PROJECT_NAME,  nullptr, nullptr); // Defining the Window Object for Vulkan to use
    }
    void createInstance() {
        // Checking if Validation Layers are supported
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers are requested but not available!");
        }
        
        // Defining Local App Info (Optional)
        VkApplicationInfo appInfo{}; // Creating Application Info
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // Defining the strucuture as application info (Required)
        appInfo.pApplicationName = PROJECT_NAME; // Defining the name of the App
        appInfo.pEngineName = "Proton Engine"; // Defining the name of the Engine
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 3); // Defining the Version of the Engine
        appInfo.apiVersion = VK_API_VERSION_1_0; // Defining the API Version of the App
        
        // Defining Global Info (Required)
        VkInstanceCreateInfo createInfo{}; // Creating Instance Info
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; // Define Instance Info
        createInfo.pApplicationInfo = &appInfo; // Giving Vulkan the app info we created
        
        // Loading and dealing with GLFW extensions
        std::vector<const char*> requiredExtensions; // Any Required Extensions EX: MacOS Compatiability
        uint32_t glfwExtensionCount = 0; // Amount of Extensions
        const char** glfwExtensions; // Extension Names
        
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // Getting all Extensions required to run the program
        
        createInfo.enabledExtensionCount = glfwExtensionCount; // Setting the amount of enabled extensions
        createInfo.ppEnabledExtensionNames = glfwExtensions; // Giving all of the enable extension names to Vulkan
        
        createInfo.enabledLayerCount = 0; // Setting Layer Count
        
        // Vulkan Extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); // Gets amount of Available Vulkan Extensions
        
        std::vector<VkExtensionProperties> extensions(extensionCount);
        
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()); // Exports them to a variable
        
        // List all availiable Vulkan Extensions
        std::cout << "Available Extensions:\n";
        
        for (const auto& extension : extensions) {
            std::cout << '\t' << extension.extensionName << '\n';
        }
        
        // MacOS Required Vulkan Extension
        if (ARCH == "APPLE") {
            for(uint32_t i = 0; i < glfwExtensionCount; i++) {
                requiredExtensions.emplace_back(glfwExtensions[i]);
            }
            
            requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            requiredExtensions.emplace_back("VK_KHR_get_physical_device_properties2");
            
            createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            
            createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
            createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        }
        
        
        // Creating the Vulkan Instance
        // Error Handling
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create instance!");
        }
        
        /// Vulkan Instance Creation
        ///     Pointer to the Instance info
        ///     Pointer to Custom memory allocater (nullptr if none)
        ///     Poniter to the variable containing the instance
    }
    
    void initVulkan() {
        createInstance();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents(); // Gather any window events such as resizing, moving, ect
        }
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr); // Destroying Vulkan Instance
        
        glfwDestroyWindow(window); // Destroying GLFW Window
        glfwTerminate(); // Terminating GLFW
    }
};


int main() {
    Application app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

