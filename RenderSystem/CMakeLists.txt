cmake_minimum_required(VERSION 3.6)

project(RenderSystem)

set(CMAKE_CXX_STANDARD 17)

set(SourceCache
        RenderSystem.cpp RenderSystem.h
        Singleton.h
        VulkanSupport.cpp VulkanSupport.h
        Prerequisites.h
        PhysicalDevice.cpp PhysicalDevice.h
        Device.cpp Device.h
        Window.cpp Window.h
        DepthBuffer.cpp DepthBuffer.h
        Buffer.cpp Buffer.h
        HardwareBuffer.cpp HardwareBuffer.h
        HardwareBufferOfUniform.cpp HardwareBufferOfUniform.h)

if (WIN32)
    set(SourceCache
            ${SourceCache}
            Win32Window.cpp Win32Window.h)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)

link_libraries(vulkan iconv)

add_library(RenderSystem SHARED ${SourceCache})