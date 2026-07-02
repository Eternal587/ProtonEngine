//
//  protonEngine.h
//  Proton Engine
//
//  Created by VibingCatt on 5/1/26.
//
#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
struct QueueFamilyIndices {
    uint32_t graphicsFamily;
};
class Application {
public:
    int windowx;
    int windowy;
    int version[3] = {0, 0, 1};
    std::string windowName = "Proton Engine";
    Application(int sizex, int sizey);
    void run();
private:
    GLFWwindow* window;
    VkInstance instance;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    int engineVersion[3] = {0, 0, 1};
    void initWindow();
    void initVulkan();
    void createInstance();
    void pickPhysicalDevice();
    void mainLoop();
    void cleanup();
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void createLogicalDevice();
};
