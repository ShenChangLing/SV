//
// Created by shenc on 2017/11/6.
//

#include "Device.h"
#include "PhysicalDevice.h"

#include "DepthBuffer.h"

#ifdef WIN32
#include "Win32Window.h"
#endif

Device::Device(PhysicalDevice* physicalDevice) :
    m_pPhysicalDevice(physicalDevice),
    m_vulkanDevice(VK_NULL_HANDLE),
    m_iDeviceQueueIndex(0)
{
    VkPhysicalDevice vkPhysicalDevice = m_pPhysicalDevice->getVkPhysicalDevice();

    m_iDeviceQueueIndex = m_pPhysicalDevice->getQueueIndexOfGraphics();

    //创建设备
    float queuePriorities[1] = {0.0f};
    VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.pNext = nullptr;
    deviceQueueCreateInfo.pQueuePriorities = queuePriorities;
    deviceQueueCreateInfo.queueCount = 1;
    deviceQueueCreateInfo.queueFamilyIndex = m_iDeviceQueueIndex;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;

    VkResult result = vkCreateDevice(vkPhysicalDevice,&deviceCreateInfo, nullptr,&m_vulkanDevice);
    if (result!= VK_SUCCESS)
    {
#ifdef RS_DEBUG
        std::cout << "vkCreateDevice Error" << result << std::endl;
#endif
    }

    //创建设备命令池
    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.queueFamilyIndex = m_iDeviceQueueIndex;
    result = vkCreateCommandPool(m_vulkanDevice,&commandPoolCreateInfo, nullptr,&m_vulkanCommandPool);
    if (result != VK_SUCCESS)
    {
#ifdef RS_DEBUG
        std::cout << "vkCreateCommandPool Error" << std::endl;
#endif
    }
}

Device::~Device()
{
    if (m_vulkanDevice)
    {
        vkDestroyCommandPool(m_vulkanDevice,m_vulkanCommandPool, nullptr);
        vkDestroyDevice(m_vulkanDevice, nullptr);
        m_vulkanDevice = VK_NULL_HANDLE;

    }
}

Window *Device::createWindow(const char *title, int x, int y, int w, int h, bool isFullScreen)
{
    Window* window = nullptr;
#ifdef WIN32
    window = new Win32Window(this,title,x,y,w,h,isFullScreen);
#endif
    return window;
}

void Device::destoryWindow(Window *window)
{
    delete window;
}

DepthBuffer *Device::createDepthBuffer()
{
<<<<<<< 6c970787d66b702c544730c7e9c7f638b127c9cd
    DepthBuffer* buffer = new DepthBuffer(this,512,512);
=======
    DepthBuffer* buffer = new DepthBuffer(this,Buffer::BufferType_Depth,512,512);
>>>>>>> be34319ba7319f2f83a014c8167252d5e121d724
    return buffer;
}

void Device::destroyDepthBuffer(DepthBuffer *depthBuffer)
{
    delete depthBuffer;
}

void Device::getVkMemoryRequirements(VkImage &image ,VkMemoryRequirements &memoryRequirements)
{
    vkGetImageMemoryRequirements(m_vulkanDevice,image,&memoryRequirements);
}
