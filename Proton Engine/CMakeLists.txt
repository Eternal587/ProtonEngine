cmake_minimum_required(VERSION 3.13)
project(protonEngineExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Required Rendering Packages
find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)

# Create Executable
add_executable(protonEngineExample
    src/main.cpp 
    lib/ProtonEngine/protonEngine.cpp
)

target_include_directories(protonEngineExample PRIVATE
    "${CMAKE_SOURCE_DIR}/lib" 
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(protonEngineExample PRIVATE 
    glfw 
    Vulkan::Vulkan
)

if(APPLE)
    target_link_libraries(protonEngineExample PRIVATE   
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    )
endif()