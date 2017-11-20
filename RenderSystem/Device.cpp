//
// Created by shenc on 2017/11/6.
//

#include "Device.h"
#include "PhysicalDevice.h"

#include "DepthBuffer.h"

#include "PipelineLayout.h"

#ifdef WIN32
#include "Win32Window.h"
#endif

Device::Device(PhysicalDevice* physicalDevice) :
    m_pPhysicalDevice(physicalDevice),
    m_vulkanDevice(VK_NULL_HANDLE),
    m_iDeviceQueueIndex(0),
    m_vulkanDescriptorPool(VK_NULL_HANDLE)
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

    //创建描述池
    VkDescriptorPoolSize descriptorPoolSize[2];//支持纹理
    descriptorPoolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorPoolSize[0].descriptorCount = 1;

    descriptorPoolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorPoolSize[1].descriptorCount =1;

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = nullptr;
    descriptorPoolCreateInfo.maxSets = 1;
    descriptorPoolCreateInfo.poolSizeCount = 2;
    descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSize;

    vkCreateDescriptorPool(m_vulkanDevice,&descriptorPoolCreateInfo, nullptr,&m_vulkanDescriptorPool);
}

Device::~Device()
{
    if (m_vulkanDevice)
    {
        vkDestroyDescriptorPool(m_vulkanDevice,m_vulkanDescriptorPool, nullptr);
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
    DepthBuffer* buffer = new DepthBuffer(this,Buffer::BufferType_Depth,512,512);
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

PipelineLayout *Device::createPipelineLayout(bool isUserTexture)
{
    PipelineLayout *pipelineLayout = new PipelineLayout(this,isUserTexture);
    assert(pipelineLayout);
    return pipelineLayout;
}

void Device::destroyPipelineLayout(PipelineLayout *pipelineLayout)
{
    assert(pipelineLayout);
    delete pipelineLayout;
}
