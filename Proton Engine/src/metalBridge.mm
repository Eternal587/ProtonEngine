//
//  metalBridge.mm
//  Proton Engine
//
//  Created by VibingCatt on 2/27/26.
//
#include <GLFW/glfw3.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
// God I fucking hate vulkan and metal

void* createMetalLayer(GLFWwindow* window) {
    NSWindow* nswindow = glfwGetCocoaWindow(window);
    
    CAMetalLayer* layer = [CAMetalLayer layer];
    
    nswindow.contentView.wantsLayer = YES;
    nswindow.contentView.layer = layer;
    
    return(__bridge void*)layer;
}
