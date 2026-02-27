//
//  main.cpp
//  Proton Engine
//
//  Created by VibingCatt on 10/5/25.
//

#include <iostream>

// GLFW & Vulkan
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstdlib>

// Strings, Text, and other basic headers
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <map>
#include <optional>

// GLM & Other Math
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <cmath>
#include <chrono>

// Audio
#include <OpenAL/OpenAL.h>

// Proton Engine Specific Headers
#include "shaderManager.h"
#include "application.h"

#if defined(_WIN32)
    #include <windows.h>
    #define VK_USE_PLATFORM_WIN32_KHR
    #define GLFW_INCLUDE_VULKAN
    #include <GLFW/glfw3.h>
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
    const std::string ARCH = "WIN32";
#elif defined(__linux__)
    // Code for Linux
    const std::string ARCH = "LINUX";
#elif defined(__APPLE__)
    #include <Cocoa/Cocoa.h>
    #include <QuartzCore/CAMetalLayer.h>
    #include <mach-o/dyld.h>
    #define VK_USE_PLATFORM_APPLE_KHR
    #define GLFW_INCLUDE_VULKAN
    #include <vulkan/vulkan_metal.h>
    #include <GLFW/glfw3.h>
    #define GLFW_EXPOSE_NATIVE_COCOA
    #include <GLFW/glfw3native.h>
    const std::string ARCH = "APPLE";
#else
#error "Architechure Platform Unknown, Proton Engine only supports Windows, MacOS, and Linux"
#endif


#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

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
    const char* PROJECT_NAME = "Proton Engine v0.0.3";
    const uint32_t WIDTH = 800; // Defining the starting Width for the GLFW Window
    const uint32_t HEIGHT = 600; // Defining the starting Height for the GLFW Window

    // Error Handling / Validation Layers
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
    VkInstance instance; // Creates the Vulkan Instance
    GLFWwindow* window; // Creates the GLFW Window Object
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // Vulkan GPU
    VkDevice device; // Vulkan Logical Device
    VkSurfaceKHR surface; // Window Surface for Vulkan to use
    
    VkQueue graphicsQueue; // Creating the Graphics Queue for all future Objects
    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Disabling GLFW from automatically attempting to use OpenGL
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Telling GLFW to disable window resizing
        
        window = glfwCreateWindow(WIDTH, HEIGHT, PROJECT_NAME,  nullptr, nullptr); // Defining the Window Object for Vulkan to use
    }
    
    void createSurface() {
        #if defined(__APPLE__)
            CAMetalLayer metalLayer = createMetalLayer(window);
            void* nativeView = glfwGetCocoaWindow(window);
            VkMetalSurfaceCreateInfoEXT createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
            createInfo.pLayer = metalLater;
        #elif defined(_WIN32)
            VkWin32SurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            createInfo.hwnd = glfwGetWin32Window(window);
            createInfo.hinstance = GetModuleHandle(nullptr);
        #endif
        
        glfwCreateWindowSurface(instance, window, nullptr, &surface);
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
        
        // Add our validiation layer info to our Instance Info
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        
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
        #if defined(__APPLE__)
            for(uint32_t i = 0; i < glfwExtensionCount; i++) {
                requiredExtensions.emplace_back(glfwExtensions[i]);
            }
            
            requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            requiredExtensions.emplace_back("VK_KHR_get_physical_device_properties2");
            requiredExtensions.emplace_back("VK_MVK_macos_surface");
            requiredExtensions.emplace_back("VK_KHR_surface");
            requiredExtensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            
            createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            
            createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
            createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        #endif
        
        
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
        pickPhysicalDevice();
        createLogicalDevice();
        createSurface();
    }
    
    void createLogicalDevice() {
        // Linking the Graphics Queue
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
        
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        
        // Defining How Important the Queue is
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        
        VkPhysicalDeviceFeatures deviceFeatures{};
        
        // Create the info for the logical Device
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        
        // Checking for Extensions and Validation Layers
        createInfo.enabledExtensionCount = 0;
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }
        
        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device!");
        }
        
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    }
    
    // Check if GPU is Suitable for Vulkan
    bool isDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indicies = findQueueFamilies(device);
        
        return indicies.isComplete();
    }
    
    void pickPhysicalDevice() {
        
        // Find GPU's with Vulkan Support
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        
        // If none support Vulkan
        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPU(s) with Vulkan Support");
        }
        
        // List all GPUS
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        
        // Testing Devices
        for (const auto& device : devices) {
            if(isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }
        
        // If no suitable device is found stop the program
        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find suitable GPU");
        }
        
    }
    
    // Graphics Queues
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        
        bool isComplete() {
            return graphicsFamily.has_value();
        }
    };
    
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;
        
        // Grabing the Amount of Queue Families
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        
        // Getting the Queue Families Properties
        std::vector<VkQueueFamilyProperties> queueFamiles(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiles.data());
        
        // Finding a Queue that support Vulkan Graphics
        int i = 0;
        for (const auto& queueFamily : queueFamiles) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }
            
            i++;
        }
        
        return indices;
    }
    
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
                    layerFound = true;
                    break;
                }
            }
            
            if (!layerFound) {
                return false;
            }
        }
        return true;
    }


    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents(); // Gather any window events such as resizing, moving, ect
        }
    }

    void cleanup() {
        vkDestroyDevice(device, nullptr); // Destroying Logical Device
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

