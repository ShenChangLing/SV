//
// Created by Administrator on 2017/11/12.
//

#include "HardwareBufferOfUniform.h"
#include "Device.h"
#include "PhysicalDevice.h"

HardwareBufferOfUniform::HardwareBufferOfUniform(Device *device, Buffer::BufferType bufferType, uint32_t bufferSize)
        : HardwareBuffer(device, bufferType, bufferSize)
{
    VkDevice vkDevice = m_pDevice->getVulkanDevice();

    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;

    bufferCreateInfo.size = m_iBufferSize;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = nullptr;

    bufferCreateInfo.flags = 0;
    bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkResult result = vkCreateBuffer(vkDevice, &bufferCreateInfo, nullptr,&m_pBuffer);

    assert(result == VK_SUCCESS);

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(vkDevice,m_pBuffer,&memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;

    bool pass = m_pDevice->getPhysicalDevice()->getMemoryTypeFormProperties(
            memoryRequirements.memoryTypeBits,0,&memoryAllocateInfo.memoryTypeIndex);
    assert(pass);

    result = vkAllocateMemory(vkDevice,&memoryAllocateInfo, nullptr,&m_pBufferMemory);
    assert(result == VK_SUCCESS);

    result = vkBindBufferMemory(vkDevice,m_pBuffer,m_pBufferMemory,0);
    assert(result == VK_SUCCESS);
}

HardwareBufferOfUniform::~HardwareBufferOfUniform()
{

}
