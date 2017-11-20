//
// Created by shenc on 2017/11/6.
//

#ifndef SV_DEVICE_H
#define SV_DEVICE_H

#include "Prerequisites.h"

class Device {
public:
    Device(PhysicalDevice* physicalDevice);
    ~Device();

    PhysicalDevice* getPhysicalDevice() const { return m_pPhysicalDevice; }
    const VkDevice& getVulkanDevice() const { return m_vulkanDevice; }

    Window* createWindow(const char* title,int x,int y,int w,int h, bool isFullScreen = false);
    void destoryWindow(Window* window);

    DepthBuffer* createDepthBuffer();
    void destroyDepthBuffer(DepthBuffer* depthBuffer);

    PipelineLayout* createPipelineLayout(bool isUserTexture = false);
    void destroyPipelineLayout(PipelineLayout* pipelineLayout);

    void getVkMemoryRequirements(VkImage &image ,VkMemoryRequirements& memoryRequirements);

private:
    PhysicalDevice *m_pPhysicalDevice;
    VkDevice m_vulkanDevice;

    VkCommandPool m_vulkanCommandPool;
    VkDescriptorPool m_vulkanDescriptorPool;

private:
    int m_iDeviceQueueIndex;
};


#endif //SV_DEVICE_H
