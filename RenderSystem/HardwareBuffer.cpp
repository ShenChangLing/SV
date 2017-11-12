//
// Created by Administrator on 2017/11/12.
//

#include "HardwareBuffer.h"
#include "Device.h"


HardwareBuffer::HardwareBuffer(Device *device, Buffer::BufferType bufferType, uint32_t bufferSize) :
    Buffer(device,bufferType),
    m_iBufferSize(bufferSize),
    m_pBuffer(VK_NULL_HANDLE),
    m_pBufferMemory(VK_NULL_HANDLE),
    m_pBufferMemoryData(nullptr)
{
}

HardwareBuffer::~HardwareBuffer()
{
    VkDevice device = m_pDevice->getVulkanDevice();
    if (m_pBuffer)
    {
        vkDestroyBuffer(device,m_pBuffer, nullptr);
        m_pBuffer = VK_NULL_HANDLE;
    }
    if (m_pBufferMemory)
    {
        vkFreeMemory(device,m_pBufferMemory, nullptr);
        m_pBufferMemory = VK_NULL_HANDLE;
    }
}

bool HardwareBuffer::lock(int offset,int size)
{
    VkResult result =
            vkMapMemory(m_pDevice->getVulkanDevice(),m_pBufferMemory,offset,size,0,(void**)&m_pBufferMemoryData);
    return result == VK_SUCCESS ? true : false;
}

void HardwareBuffer::unlock()
{
    vkUnmapMemory(m_pDevice->getVulkanDevice(),m_pBufferMemory);
    m_pBufferMemoryData = nullptr;
}

void HardwareBuffer::writeData(size_t offset, size_t length, const void *source)
{
    if (m_pBufferMemoryData)
    {
        memcpy(m_pBufferMemoryData+offset,source,length);
    }
}

void HardwareBuffer::readData(size_t offset, size_t length, void *dest)
{
    if (m_pBufferMemoryData)
    {
        memcpy(dest,m_pBufferMemoryData+offset,length);
    }
}

