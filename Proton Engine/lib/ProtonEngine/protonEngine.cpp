//
//  protonEngine.cpp
//  Proton Engine
//
//  Created by VibingCatt on 5/1/26.
//

#include <iostream>

// GLFW & Vulkan
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstdlib>

#ifndef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"
#endif

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
#include <limits>

// Audio
#include <OpenAL/OpenAL.h>

// Proton Engine Specific Headers
#include "protonEngine.h"

// Platform Headers
#if defined(_WIN32)
    #define GLFW_INCLUDE_VULKAN
    #define VK_USE_PLATFORM_WIN32_KHR
    #include <GLFW/glfw3.h>
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
    const std::string ARCH = "WIN32";
#elif defined(__linux__)
    const std::string ARCH = "LINUX";
#elif defined(__APPLE__)
    #define GLFW_INCLUDE_VULKAN
    #include <GLFW/glfw3.h>
    const std::string ARCH = "APPLE";
#else
#error "Architechure Platform Unknown, Proton Engine only supports Windows, MacOS, and Linux"
#endif

// GLFW & Vulkan

// Vulkan overhead & Debug
const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation"
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        
    
        return VK_FALSE;
}

#ifdef NDEBUG
const bool enableValidiationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    
    
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

// Application Class Functions
Application::Application(int sizex, int sizey) : windowx(sizex), windowy(sizey) {
    
}
void Application::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}
void Application::initWindow() {
    if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Makes GLFW Use Vulkan
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  // Disables Window Resizing
    
    window = glfwCreateWindow(windowx, windowy, windowName.c_str(), nullptr, nullptr); // Creating window object
    
    if (!window) {
            throw std::runtime_error("Failed to create GLFW window");
        }
}
void Application::initVulkan() {
    createInstance();
    pickPhysicalDevice();
    createLogicalDevice();
}

void Application::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    std::vector<const char*> deviceExtensions;
    #if defined(__APPLE__)
        deviceExtensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
    #endif

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.empty() ? nullptr : deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }
    vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
}

void Application::createInstance() {
    // Check Vulkan Validation Layers
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("Validation layers requested, but not available");
    }
    
    // Application Info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = windowName.c_str(); // Default: "Proton Engine" can be changed during runtime
    appInfo.applicationVersion = VK_MAKE_VERSION(version[0], version[1], version[2]);  // Default: 0, 0, 1 can be changed during runtime
    appInfo.pEngineName = "Proton Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(engineVersion[0], engineVersion[1], engineVersion[2]);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    // Create Info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    if(enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }
    
    // Process GLFW extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    
    std::vector<const char*> requiredExtensions;
    
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    
    for(uint32_t i = 0; i < glfwExtensionCount; i++) {
        requiredExtensions.emplace_back(glfwExtensions[i]);
    }
    
    if (ARCH == "APPLE") {
        // REQUIRED EXTENSIONS FOR USING MOLTENVK - APPLE VULKAN PORTABILITY
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }
    
    if (enableValidationLayers) {
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    
    std::cout << "Available Extesnions:\n";
    
    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }
    
    // Check for errors & create instance
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create vulkan instance!");
    }
}

int rateDeviceSuitability(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 0;

    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    // Application can't function without geometry shaders
    if (!deviceFeatures.geometryShader && ARCH != "APPLE") {
        return 0;
    } else if (!deviceFeatures.geometryShader && ARCH == "APPLE") {
        std::cout << "Warning: Apple Silicon devices do not support geometry shaders, Proceeding anyway\n";
    }

    return score;
}
void Application::pickPhysicalDevice() {
    physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan Support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    std::multimap<int, VkPhysicalDevice> candidates;
    for (const auto& device : devices) {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    if (candidates.rbegin()->first > 0) {
        physicalDevice = candidates.rbegin()->second;
    } else {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}
static const std::vector<const char*> requiredDeviceExtensions = {
#ifdef __APPLE__
   VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME,
#endif
};
bool Application::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    for (const char* required : requiredDeviceExtensions) {
        bool found = false;
        for (const auto& ext : availableExtensions) {
            if (strcmp(required, ext.extensionName) == 0) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}
bool Application::isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    QueueFamilyIndices indices = findQueueFamilies(device);
    
    if (indices.graphicsFamily == std::numeric_limits<uint32_t>::max()) {
        std::cout << "Device Name: " << deviceProperties.deviceName << " - Rejected (No Graphics Queue)\n";
        return false;
    }

    // Accept other GPUS (incliuding integrated) including on Apple Silicon 
    bool typeOkay =
        deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
        deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ||
        deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ||
        deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER;

    if (!typeOkay) {
        std::cout << "Device Name: " << deviceProperties.deviceName << " - Rejected (Unsupported Device Type)\n";
        return false;
    }

    // Require geometry shader support (except for on Apple Silicon)
    if (ARCH != "APPLE" && !deviceFeatures.geometryShader) {
        std::cout << "Device Name: " << deviceProperties.deviceName << " - Rejected (No Geometry Shader Support)\n";
        return false;
    } else if (ARCH == "APPLE" && !deviceFeatures.geometryShader) {
        std::cout << "Device Name: " << deviceProperties.deviceName << "\n";
        std::cout << "Warning: Apple Silicon devices do not support geometry shaders, Proceeding anyway\n";
    }

    // Check for MoltenVK portability subset support on Apple Silicon
    if (ARCH == "APPLE" && !checkDeviceExtensionSupport(device)) {
        std::cout << "Device Name: " << deviceProperties.deviceName << " - Rejected (Missing Required Extensions for Apple Silicon)\n";
        return false;
    }
    return true;
}
QueueFamilyIndices Application::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
            break;
        }
        i++;
    }

    return indices;
}
void Application::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}
void Application::cleanup() {
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

